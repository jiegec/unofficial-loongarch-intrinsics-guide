# Buffer Objects

A buffer object is the unit of GPU memory allocation: a size, a handle and a
place in memory. Three steps make it usable, and their lifetimes differ. The
creation request fixes the domains and flags. `GEM_MMAP` returns an offset that
`mmap` turns into a CPU pointer. `GEM_VA` puts a range of the buffer into the
GPU's address space, and only then can a command stream name it.
[User-space interface](uapi.md#buffer-objects) fixes the argument layout of every
ioctl named here.

## Domains

| Domain | Value | Effect |
|--------|------:|--------|
| `LOONGGPU_GEM_DOMAIN_CPU` | `0x1` | System memory that the GPU cannot address. |
| `LOONGGPU_GEM_DOMAIN_GTT` | `0x2` | System memory with a GPU address, reachable through the GART. |
| `LOONGGPU_GEM_DOMAIN_VRAM` | `0x4` | Local memory. Only its CPU-visible part can be mapped by the CPU. |
| `LOONGGPU_GEM_DOMAIN_GDS` | `0x8` | Accepted, then ignored: no placement of its own. |
| `LOONGGPU_GEM_DOMAIN_GWS` | `0x10` | Accepted, then ignored. |
| `LOONGGPU_GEM_DOMAIN_OA` | `0x20` | Accepted, then ignored. |
| `LOONGGPU_GEM_DOMAIN_DOORBELL` | `0x40` | Refused with `-EINVAL`. |

GDS, GWS and OA name on-chip resources but have no placement of their own, so a
request that names only them produces system memory. A client MUST NOT expect such a
buffer to be shared memory, a wave semaphore or an append counter.

A request MAY name several domains. The placement list is built in a fixed order —
VRAM, then GTT, then system memory — and the kernel takes the first that fits. A
failure is retried by widening the request: a VRAM request that fails is retried as
VRAM and GTT unless compression was asked for, and a failed `CPU_ACCESS_REQUIRED`
request is retried without that flag. The placement a client receives can therefore
be weaker than the one it asked for. A buffer whose allowed set is exactly VRAM also
gains GTT, so it can be migrated to system memory; `DISCARDABLE` and
`VM_ALWAYS_VALID` suppress that widening.

## Creation flags

| Flag | Value | Effect |
|------|------:|--------|
| `CPU_ACCESS_REQUIRED` | `1 << 0` | Confine the VRAM placement to the CPU-visible window. |
| `NO_CPU_ACCESS` | `1 << 1` | Refuse every CPU mapping; `GEM_MMAP` answers `-EPERM`. |
| `CPU_GTT_USWC` | `1 << 2` | Write-combined for the GTT and system placements. |
| `VRAM_CLEARED` | `1 << 3` | Zero the buffer after allocation, when it landed in VRAM. |
| `VM_ALWAYS_VALID` | `1 << 6` | Tie the buffer to the VM root's reservation. |
| `EXPLICIT_SYNC` | `1 << 7` | The client, not the kernel, orders shared access. |
| `COMPRESSED_MASK` | `0x7 << 9` | Compression mode; the value enters the page-table entry. |
| `DISCARDABLE` | `1 << 13` | The contents need not survive an eviction. |

`CPU_ACCESS_REQUIRED` is a hint, not a property: it steers the first allocation
into the visible part of VRAM and is then removed from the flag word, so a
create-info query taken before the first CPU fault does not report it.
`NO_CPU_ACCESS` changes no placement.

`VM_ALWAYS_VALID` gives the buffer the VM root's reservation instead of its own.
Its entries are written into the VM's update stream, it is not validated by a
residency list, it cannot be exported and it cannot acquire a handle in a second
VM. In VRAM it is placed with no-evict, so allocation can fail rather than migrate
the buffer. `COMPRESSED_MASK` is accepted only when the domain is exactly VRAM and
the compression engine is initialised. `DISCARDABLE` leaves the eviction path with
no destination: the contents are dropped, not migrated.

Four bits are defined and refused by `GEM_CREATE` with `-EINVAL`: `SHADOW`
(`1 << 4`) and `VRAM_CONTIGUOUS` (`1 << 5`), which only kernel-internal
allocations use; `MQD_GFX9` (`1 << 8`), which has no user at all; and
`PREEMPTIBLE` (`1 << 12`), which only the compute memory path uses. A client MUST
treat any of them, and any undefined bit, as an error rather than as a request that
was applied. `NO_CPU_ACCESS` with `CPU_ACCESS_REQUIRED` is accepted but
contradictory, and a client MUST NOT set both.

The size is rounded up to the host page size without wrapping, and a request that
rounds to zero or overflows is refused. The alignment is raised to at least the page
size and is the buffer's physical start alignment, not one of its GPU address.

## CPU mapping

`GEM_MMAP` returns an offset into the file's offset space, not a pointer; the
client passes it to `mmap` with `MAP_SHARED`, and a mapping longer than the buffer
is refused. Pages are faulted in on first touch, and a page that is not in a
CPU-reachable placement moves the buffer first: into the visible part of VRAM if
there is room, otherwise to GTT. A pinned buffer cannot be moved and the fault
fails instead.

What the CPU may assume depends on the placement. VRAM in the visible window is
uncached and write-combined, so a CPU store may still sit in a write-combining
buffer after the store retires. GTT and system memory are cached by default, so a
CPU store may sit in a CPU cache and a CPU load may return a cached copy of memory
the GPU has since written; with `CPU_GTT_USWC` they are uncached and
write-combined, and CPU reads fetch memory.

No interface operation flushes a CPU cache or a write-combining buffer. Before a
CPU read of bytes the GPU wrote, a client MUST wait for the submission that wrote
them; nothing else establishes that the write is visible. Before a GPU read of
bytes the CPU wrote, those writes MUST have reached memory through a mapping whose
caching policy allows it, or through the buffer's CPU-access bracket.

> UNRESOLVED: whether a cached GTT or system mapping is coherent with GPU reads on
> this generation, and whether anything in the submission path writes the CPU cache
> back. The [memory system](../architecture/memory-system.md) reaches the same
> question from the address side. A client that needs a guarantee SHOULD request
> `CPU_GTT_USWC` or allocate in VRAM.

## GPU mapping

`GEM_VA` has four operations: `MAP` adds a range, `REPLACE` removes whatever
covers the range and then adds the new mapping, `UNMAP` removes the one mapping
that starts at `va_address`, and `CLEAR` removes every mapping in a range and
needs no handle. `va_address` is always a multiple of the GPU page size, and for
`MAP`, `REPLACE` and `CLEAR` so is `map_size`; `MAP` and `REPLACE` also
require a page-aligned `offset_in_bo`. The page size is 4 KiB or 16 KiB depending
on how the kernel was built, and the window, its floor and its hole are described in
the [memory system](../architecture/memory-system.md). `MAP` requires the range to be
free, and an overlap is refused with `-EINVAL`. `UNMAP` answers `-ENOENT` when
this buffer has no mapping that starts at the address. `UNMAP` and `CLEAR` are
refused with `-EBUSY` for a range that covers a live user compute queue's ring or
index block, which must stay mapped until the queue is freed.

Of the flag bits only `READABLE` (`1 << 1`, the present bit) and `WRITEABLE`
(`1 << 2`, the write bit) reach the page-table entry. `EXECUTABLE` (`1 << 3`)
and the memory-type field (`0xf << 5`) have no field in this entry format and
change nothing. `PRT` (`1 << 4`) maps the VM's partial-residency object instead
of the handle, and `DELAY_UPDATE` (`1 << 0`) records the mapping without
publishing the page directory; it cannot be combined with `PRT` or with any page
flag.

A mapping the GPU will read through MUST carry `READABLE`; without it the entry is
written with the present bit clear, so the range faults. `EXECUTABLE` is neither
required nor representable: an indirect buffer is admitted because its mapping is
present, not because of any execute attribute.

`MAP` and `REPLACE` write their entries and publish the directory before they
return, unless `DELAY_UPDATE` leaves the work to the next submission; a failed
write removes the mapping again, so a client never observes a mapping that exists
but does not translate. Removing a mapping is not immediate: `UNMAP` and `CLEAR`
take the range out of the VM's tree and queue the page-table clear, which a later
VA call or the next submission publishes, ordered after the buffer's bookkeeping
fences as they stood when the range was removed. A running submission therefore
keeps its translation. That grace is not permission to reuse the address: a client
MUST NOT map different contents at a range until the submissions that named it have
retired, and retirement is reported by the submission, not by the buffer; see
[Waiting](#waiting).

> UNRESOLVED: whether a partial-residency mapping translates at all. The range is
> recorded against a per-VM object that has no buffer, and the page-table update for
> an object without a buffer writes no flags, so the entries are written not present.

## Import, export and user pointers

Export produces a DMA-BUF file descriptor and is refused with `-EPERM` for a
user-pointer buffer and for a `VM_ALWAYS_VALID` buffer. A buffer imported from
another device carries the exporter's reservation and is restricted to GTT. While
another device holds an attachment the buffer is pinned in GTT and counted as
shared, and `SET_PLACEMENT` refuses to move a shared buffer to VRAM. Sharing is
explicit beyond the device: a client that maps a shared buffer for the CPU MUST
bracket the access with the DMA-BUF CPU-access protocol, which is also the only
path that asks the exporter to make its own caches coherent.

`GEM_USERPTR` registers a range of the calling process's address space as a
buffer. Address and size are page aligned; a registration that may be written
through MUST set `REGISTER` to install the invalidation notifier, and one with
neither `READONLY` nor `REGISTER` is refused with `-EACCES`. `VALIDATE`
fetches and places the pages immediately instead of waiting for the first
submission. The result is a normal handle with four restrictions: the CPU cannot map
it, it cannot be exported, its placement cannot be changed, and only the process
whose address space it came from can put it in a residency list. Before a
submission the kernel re-acquires pages the notifier invalidated, retrying a bounded
number of times before failing with `-EDEADLK`. The interface declares itself
unreliable and tells a client to keep a fallback that performs the operation
without it; a client MUST NOT make a user-pointer buffer the only path to a result.

> UNRESOLVED: what a client may rely on when it unmaps a registered range while a
> submission that used it is still running. The invalidation callback waits for the
> buffer's read-level fences before releasing the pages, but a submission records
> its own fence on the buffer at bookkeeping level, which that wait does not cover.

## Waiting

`GEM_WAIT_IDLE` takes a handle, a reserved flag word and a deadline. The deadline
is absolute `CLOCK_MONOTONIC` nanoseconds; a value with the top bit set, such as
`~0ull`, means no deadline, and one already in the past returns at once. The output
reports `status` 0 when the buffer is idle and 1 when the deadline passed first.
The ioctl returns 0 in both cases, and a negative value when the wait was
interrupted or the handle is unknown. Its second output word is written as 0 and
says nothing about the buffer's domain.

The wait covers the fences that guard read and write access to the buffer. A
submission records the fence it produces on each buffer it validated at bookkeeping
level, the level that carries no implicit synchronization, and a read-level wait
does not include it. An idle status therefore means that no read-level or
write-level user is outstanding, not that the last submission naming the buffer has
finished. A client that needs a submission's results MUST wait on that submission —
its fence, `WAIT_CS` or `WAIT_FENCES` — or ask the retirement query
`FENCE_STATUS`, which answers whether the submission can still access its buffers.

The kernel does not rely on the idle wait for its own teardown: closing a handle,
and dropping a buffer's last reference, wait for the buffer's bookkeeping fences with
a bound before the page-table entries and the backing store go away. A close while a
submission still names the buffer therefore does not lose the data it is reading.
The bound is host policy, ten seconds by default, and a timeout is reported rather
than turned into an unbounded sleep.

## Tiling metadata

`GEM_METADATA` attaches a tiling word and up to 256 bytes of opaque data to a buffer.
The word is packed as follows:

| Bits | Field |
|------|-------|
| `[3:0]` | `ARRAY_MODE` |
| `[8:4]` | `PIPE_CONFIG` |
| `[11:9]` | `TILE_SPLIT` |
| `[14:12]` | `MICRO_TILE_MODE` |
| `[16:15]` | `BANK_WIDTH` |
| `[18:17]` | `BANK_HEIGHT` |
| `[20:19]` | `MACRO_TILE_ASPECT` |
| `[22:21]` | `NUM_BANKS` |

An alternative encoding places a swizzle mode in `[4:0]`. The two overlap and the
kernel cannot distinguish them: it stores the word and returns it unchanged, so a
client MUST know which encoding it wrote and MUST NOT set bits from both. Only
`ARRAY_MODE` is read: the scanout path accepts 0 (linear) and 2 (tiled 4x4) and
refuses any other value at flip time, so a display buffer MUST carry the array mode
its layout uses. The other fields have no reader, and a page-table entry's
compression bits come from `COMPRESSED_MASK` at creation, not from this word.

## Residency lists

A submission names the buffers it may touch through a residency list, a per-file
object with its own handle space:

| Operation | Value | Effect |
|-----------|------:|--------|
| `CREATE` | 0 | Builds a list and returns a handle, never 0. |
| `DESTROY` | 1 | Drops the list; an unknown handle is not an error. |
| `UPDATE` | 2 | Replaces the entries of an existing list; an unknown handle is `-ENOENT`. |

Each entry is a buffer handle and a priority from 0 to 32; a larger value is
clamped. The list is ordered by descending priority, stably within one priority.
That order is the order in which a submission reserves and places the buffers, and
the last entry is the one a submission may evict under memory pressure, so a higher
priority means placed earlier and evicted later: zero is the default and the first
eviction candidate.

Each entry takes its own reference and a submission holds the list until it
finishes, so destroying a list after a submission was admitted is safe. A handle may
appear more than once; the list does not merge entries. A submission may also carry
its list inline as a chunk instead of naming a handle, and one that does both is
refused; when it names neither, the submission validates only its page directory
and its own implicit buffers.

Before a submission is admitted, every listed buffer without a parent is reserved
and placed in one of its allowed domains, its page tables are written and their
fences ordered before the job, and the VM's page directory joins the same set. An
indirect buffer outside the set is refused even when its mapping is present. Buffers
created with `VM_ALWAYS_VALID` are the exception: they have a parent, they stay out
of the set, and their page tables are kept current by the VM.

## Lifecycle

The buffer and its mappings have separate lifetimes:

1. `GEM_CREATE` allocates the buffer, returns a handle, and associates the buffer
   with the calling file's VM.
2. `GEM_MMAP` and `mmap` add a CPU mapping of the object, independent of any GPU
   view of it; `GEM_VA` with `MAP` adds a virtual range and its page tables.
3. A residency list and a submission place the buffer and queue the work.
4. The client waits on the submission's fence, not on the buffer's idle status.
5. `UNMAP` or `CLEAR` retires the range, and closing the handle removes that
   file's mappings and drops one reference.

A range can be unmapped while the buffer lives on, and the buffer can outlive every
mapping of it; a mapping without a buffer exists only for the partial-residency
object. The memory is freed with the last reference, which may be a residency list,
an import or the handle. The free waits for the buffer's fences under the same bound as
the close path, so closing a handle early does not force the GPU to read freed
memory, and a client that keeps a list alive after closing the handle keeps the
buffer alive with it.
