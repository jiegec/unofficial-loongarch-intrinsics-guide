# Memory System

A client of the LG200 sees one flat 64-bit address. Turning it into memory takes
four independent decisions: the aperture tag in the high 16 bits, the client walk
below 1 TiB, the VMID that selects which page directory the walk starts from, and
the buffer-object mapping that fills that directory. This chapter gives the
numbers behind each decision, the page-table entry format, and the rules that make
a write visible and a fault legible.

![GPU virtual address space](../assets/fig-va.svg)

The figure plots two independent structures: the client window with its floor and
hole, and the decode apertures, which are not ranges inside it.

## Address space

The walk covers **2^40 bytes, 1 TiB**. A client can map only the part above a
1 MiB floor, so the usable window is 1 TiB − 1 MiB.

| Lower bound | Upper bound (exclusive) | Meaning |
|---|---|---|
| `0x0000_0000_0000_0000` | `0x0000_0000_0010_0000` | Floor, 1 MiB. Never mapped; a request is refused. |
| `0x0000_0000_0010_0000` | `0x0000_0100_0000_0000` | Client window. The only addresses a client can map. |
| `0x0000_0100_0000_0000` | `0x0000_8000_0000_0000` | 1 TiB to 2^47. Above the walk; a request is refused. |
| `0x0000_8000_0000_0000` | `0xffff_8000_0000_0000` | Hole, from 2^47. Refused outright. |
| `0xffff_8000_0000_0000` | `0x1_0000_0000_0000_0000` | High canonical. Reduced to bits 47:0, then judged as above. |

A request passes three checks in order: refused below the floor, refused inside
the hole, then reduced to its low 48 bits and refused again if the result is
below the floor or if `address + size` exceeds 1 TiB.

The reduction makes the high canonical range an alias of the low form:
`0xffff_8000_0010_0000` and `0x0000_0000_0010_0000` both name the page at 1 MiB.
It cannot widen the window, because the reduced value is still compared with
1 TiB, and it cannot reach an aperture, because a reduced value below 1 TiB never
has a non-zero tag.

The device-information query publishes a base of 1 MiB
(`0x0000_0000_0010_0000`) and an exclusive end of 1 TiB − 1 MiB
(`0x0000_00ff_fff0_0000`), because it subtracts the floor from the range size.
Its window is therefore 1 TiB − 2 MiB wide. A driver MUST take its allocator range
from the query and MUST NOT map between that end and 1 TiB even though the ioctl
would accept it.

## Page tables

A page-table entry is 8 bytes. The client walk has three levels; the page size is
4 KiB or 16 KiB, follows the host page size, and is fixed when the kernel is built.
Both page sizes describe the same 40 bits with the split moved.

| Page size | Level | Width | Shift | Entries | One entry covers |
|---|---|---|---|---|---|
| 4 KiB | root | 10 | 30 | 1024 | 1 GiB |
| 4 KiB | middle | 9 | 21 | 512 | 2 MiB |
| 4 KiB | leaf | 9 | 12 | 512 | 4 KiB |
| 16 KiB | root | 4 | 36 | 16 | 64 GiB |
| 16 KiB | middle | 11 | 25 | 2048 | 32 MiB |
| 16 KiB | leaf | 11 | 14 | 2048 | 16 KiB |

The geometry is programmed per VMID as three width/shift pairs in one register:
the widths occupy bits 31:26, 19:16 and 9:6, the shifts bits 25:20, 15:10 and 5:0,
root pair first. A client does not program it, but the split decides which level a
fault address falls into and how large a contiguous mapping can be before it needs
another intermediate table. VMIDs 1–7 get the three-level geometry above; VMID 0,
which carries kernel traffic, gets a single level over the GART.

An entry has the following fields.

| Bits | Field | Meaning |
|---|---|---|
| `[63:40]` | Reserved | Not part of the address. |
| `[39:12]` | Physical page base | Page frame, 4 KiB aligned. |
| `[11:8]` | Undefined | Written zero. |
| `[7:4]` | ZINF | Compression or zero-infill control. |
| `[3]` | W | Writable. |
| `[2]` | Exception | Raise an exception on access. |
| `[1]` | Huge | Huge-page entry. |
| `[0]` | Present | Entry translates. |

A client mapping sets two fields and no others: readable sets `Present`, writable
also sets `W`. The executable, partially-resident and memory-type flags of the
mapping request are accepted by the interface and not encoded in the entry, so
they cannot change how the MMU treats a page, and a client cannot request a
huge-page, exception or compressed entry at all.

> UNRESOLVED: whether the MMU honours the huge-page bit for a client mapping, and
> what `ZINF` controls on this generation. Neither is reachable from the mapping
> interface, so a driver does not need the answer to map memory, but it does need
> it to interpret an entry it did not write.

## VMIDs

The device has eight VMID contexts. A VMID names a page-directory root and the
PASID last bound to it.

| VMID | Owner | Use |
|---|---|---|
| 0 | Kernel | Control traffic and page-table updates; single-level GART geometry. |
| 1–3 | Client processes | Graphics, blit and transfer submissions; three-level walk. |
| 4–7 | Compute processes | Compute queues; three-level walk. |

A client process draws from three contexts, VMIDs 1 to 3. VMID 0 is never in the
pool, and 4–7 are sealed to the compute side when the pipes are partitioned, so a
graphics process cannot be given one. With more live client processes than VMIDs,
a VMID changes owner between submissions.

Reprogramming a page-directory base is the dangerous operation here: a walk
already in the pipeline when the base changes reads the new directory with an old
address and finds no entry. Three rules limit the damage.

- A submission waits for the previous owner's work before a VMID is handed to a
  different process; a VMID reused by the same process does not wait.
- A returned VMID is preferred for reuse only after 20 ms. The delay is a
  preference, not a hold: when every VMID is fresh, the oldest is taken anyway.
- A base is rewritten only when it changed, but the invalidation that follows it
  is emitted on every submission, because the page tables are what may have
  changed.

At most two VMIDs may be held reserved on the device at once, whichever process
asks; the third stays in the pool so that a process which reserves nothing always
finds one.

Creating a user-mode compute queue requires a reserved VMID, and unreserving is
refused while such a queue exists, so a client that owns one MUST reserve a VMID
first and keep it until the queue is freed.

A submission also tells the firmware which PASID now owns its VMID, through a bind
command on the ring. That record is what lets a fault be attributed when the
interrupt vector carries only a VMID.

## Segment apertures

Two decode tags sit outside the client window. An address whose bits 63:48 equal a
tag is redirected before any page table is consulted.

| Aperture | Tag and base | Limit | Effect |
|---|---|---|---|
| LDS | tag `0x0001`, base `0x0001_0000_0000_0000` | `0x0001_ffff_ffff_ffff` | Redirects to on-chip workgroup shared memory. |
| Scratch | tag `0x0002`, base `0x0002_0000_0000_0000` | `0x0002_ffff_ffff_ffff` | Redirects to per-invocation private memory, expanded by work-item id. |

Each aperture spans 4 GiB, which is also the span an unsigned 32-bit byte offset
can address. The base reaches the engine as one 32-bit word holding bits 63:32 of
the base: `0x0001_0000` for LDS and `0x0002_0000` for scratch. The low 32 bits are
zero and are not part of the tag.

A client page table never maps either aperture. Its walk covers 40 bits, so no
entry in it can be reached by a tagged address, and the redirect is a decode, not
storage. Scratch bytes and shared memory therefore arrive by two different routes.

- The **scratch aperture** is the address a shader uses for spilled registers. The
  bytes are private memory the hardware expands per work-item; no page table is
  involved.
- A **graphics-stage spill buffer** is an ordinary buffer object at an ordinary
  client address, reached through a buffer descriptor. It uses no aperture.

LDS needs no buffer either: the redirect lands in the on-chip memory of the
compute unit, 128 KiB per unit, reported in the topology properties.

The size of a workgroup's LDS window is declared in the launch record, not in a
page table. The field is `granulated_shm_size`, bits 20:10 of the resource word,
in 128-byte granules carrying a `0x3f8` bias:

```text
granulated_shm_size = (shared_bytes * 8 + 0x3f8) & 0x1ffc00
```

Eleven bits name at most 2047 granules, so the largest representable window is
262,016 bytes; a larger declared size would be encoded as a smaller one. The
advertised per-workgroup limit is 16 KiB, which fits both this field and the 4 GiB
aperture span.

> UNRESOLVED: the actual per-workgroup LDS capacity, and how many workgroups fit
> in the 128 KiB of a compute unit at a given window size. The launch record can
> name a window far larger than the advertised limit, and nothing in the sources
> says what the hardware does with such a request.

## Buffer objects and mappings

A buffer object is an allocation with a size, a placement and a handle. A
placement is system memory, GTT (system memory made GPU-accessible) or VRAM; the
domain flags also name on-chip blocks (GDS, GWS, ordered append) and a doorbell
page, which are not client-window memory.

A buffer object becomes reachable from a shader when it is mapped. Four operations
share one request structure:

| Operation | Value | Contract |
|---|---|---|
| `LOONGGPU_VA_OP_MAP` | 1 | Create a mapping; the range MUST be free. |
| `LOONGGPU_VA_OP_UNMAP` | 2 | Remove the mapping that starts at the given address; size is 0. |
| `LOONGGPU_VA_OP_CLEAR` | 3 | Remove every mapping that overlaps the range; no handle is needed. |
| `LOONGGPU_VA_OP_REPLACE` | 4 | Remove the overlapping mappings, then create this one. |

The request carries a virtual address, an offset inside the object, a size and a
flag word. Address, offset and size MUST each be aligned to one GPU page, the
alignment the device-information query reports. `offset + size` MUST fit inside
the object, and `address + size` MUST lie inside the client window. MAP, REPLACE
and CLEAR require a non-zero size; UNMAP takes only an aligned start address,
because it names one mapping.

The flag word MUST be a subset of one of two sets:
`LOONGGPU_VM_DELAY_UPDATE` with any of `LOONGGPU_VM_PAGE_READABLE`,
`LOONGGPU_VM_PAGE_WRITEABLE`, `LOONGGPU_VM_PAGE_EXECUTABLE` and the memory-type
mask, or `LOONGGPU_VM_DELAY_UPDATE` with `LOONGGPU_VM_PAGE_PRT`. A word that mixes
the two, or sets any other bit, is refused.

The page tables are written by the request itself, so a client may map a buffer
and immediately submit work against the address. `LOONGGPU_VM_DELAY_UPDATE`
defers only the publication of the directory tree to the next submission.

The ring and index block of a live user-mode compute queue are leased: UNMAP,
CLEAR and REPLACE return `-EBUSY` for a range that overlaps them, and the lease
ends only when the queue is freed.

## Coherency and ordering

Visibility has four layers. A client has to satisfy all four; no one substitutes
for another.

| Layer | Mechanism | Obligation |
|---|---|---|
| Residency | The submission names its buffers; they are validated and made resident before the ring is kicked, and a validated buffer is not moved until the submission retires. | A buffer MUST be in the submission's list before the GPU touches it. |
| Publication | A completed ring frame is followed by a full barrier before the write pointer becomes visible. | A client that writes its own queue's write pointer MUST order its preceding stores before the index store, because that store is the publish. |
| Completion | The submission's fence signals after the command processor retires the work. | A CPU read of GPU-written memory is valid only after the fence. |
| Shader | `check` drains pending work named by four domain bits; `barrier` synchronises a workgroup's shared memory. | Order the invocation's own stores with `check`, and shared memory with `barrier`. |

There is no cache-management packet and no invalidate command, so a client cannot
flush a cache before a submission; the completion fence is the only ordering point
the interface offers between GPU writes and a CPU read.

`check` drains four independent domains — kernel and scalar (bit 0), vector memory
(bit 1), vector store (bit 2), shared memory (bit 3) — and the bits may be
combined. It orders the work of the current invocation only: it is not a host
fence, it does not signal completion, and it does not order another invocation's
stores. `barrier` is the shared-memory ordering mechanism inside a workgroup; it
too is not a global fence.

Placement decides what the CPU may assume. VRAM is uncached and write-combined.
GTT and system memory are cached by default, or uncached write-combined when the
creation flags request it.

A write-combined mapping MAY still hold a store after the store instruction
retires, and a cached mapping MAY hold it in a CPU cache. The interface provides
no flush for either case, so a client that needs the GPU to observe a CPU store
through such a mapping has only submission order to rely on.

> UNRESOLVED: whether a cached GTT mapping is coherent with GPU reads on this
> generation, and whether the CPU cache is written back anywhere in the submission
> path. A client that needs a guarantee SHOULD use an uncached write-combined
> mapping or a VRAM buffer, and SHOULD NOT write bytes the GPU may be reading.

## Atomics

An atomic instruction performs a read-modify-write that is indivisible with
respect to other atomics on the same location. Four address spaces are available,
and each maps onto an address form defined above:

| Space | Address form |
|---|---|
| Shared memory | Offset inside the LDS aperture, tag `0x0001`. |
| Buffer | Buffer descriptor plus index and offset. |
| Flat | Absolute 64-bit address: a client VA, or a tagged aperture address. |
| Image | Image descriptor plus a coordinate window. |

The operation set, the destination tuple and the per-operation semantics belong to
the instruction-set chapters. Two consequences of the address space matter here.
An atomic to a client address follows the same mapping and residency rules as a
load, so an address with no present entry faults. An atomic to a shared-memory
offset is redirected by the tag and needs no mapping, but the offset MUST lie
inside the window the launch declared: an access past that window faults inside
the LDS aperture.

> UNRESOLVED: the service order for several lanes of one instruction that target
> the same location, and the ordering of an atomic against other memory
> instructions of the same invocation beyond what `check` states. A producer that
> needs a defined order between two atomics MUST express it with `check` or
> `barrier` rather than rely on issue order.

## Faults and recovery

An access the MMU cannot translate raises a fault. The report is a 16-byte
interrupt vector with fixed fields:

| Field | Bits | Contents |
|---|---|---|
| Source id | `dw0[7:0]` | Which fault line fired. |
| PASID | `dw0[23:8]` | 16 bits; MAY be 0 even when the faulting process has a PASID. |
| VMID | `dw0[31:28]` | The VMID the pipeline was bound to. |
| Fault address | `dw3[15:0]`, `dw2[31:0]` | Bits 47:32 and 31:0 of the faulting address. |
| Status | `dw3[31:16]` | Fault status word. |

`dw1` is not defined and is not read. The report names an address, a VMID and a
status; it does not say whether the access was a read or a write, and the fault
record the compute interface polls carries no access type either.

A PASID of 0 is normal: the field exists, but a fault may arrive without one, and
the owner is then resolved from the VMID's last binding. A client MUST NOT treat
the reported PASID as an authoritative owner, and MUST NOT treat a VMID as stable
across submissions, because both are recycled.

Reporting is rate-limited per VM. A VM may deliver 16 faults; when the budget is
exhausted the device stops delivering that VM's fault interrupts for 1000 ms and
restores the budget afterwards. A fault that is never delivered leaves the
faulting job stalled until its ring times out, so a client that provokes a burst
of faults loses the diagnosis, not just the log line.

Whether a fault is reported at all depends on how the MMU is configured. With
retries enabled the MMU retries the access itself and reports nothing, which keeps
the damage inside the faulting queue. With retries disabled the fault is reported
and the faulting engine waits for the host; no resume path exists for that engine,
so the wait ends as a ring timeout.

A fault is not automatically fatal when the VM removed the address itself. If the
page tables no longer contain it, nothing of that VM is in flight, and the removal
happened within the last 5 s, the report is counted and dropped instead of
quarantining the pipe, at most 64 times per boot. That is a host policy, not a
licence to touch an unmapped address: the evidence requirement is exact and the
budget finite.

Recovery closes the device to new work and reopens it. A reservation, a doorbell
ring, or a page-table update prepared before the closure is stale: the admission
epoch a publication samples will have moved, and the doorbell is refused rather
than rung. A client MUST treat that refusal as a retry, and MUST NOT assume that
work admitted before a reset was executed after it; recovery retires those
submissions rather than replaying them.

> UNRESOLVED: which faults are recoverable per pipe, and how a compute queue's
> fault affects an unrelated graphics context. The fault record and the retry
> mechanism are documented; the reach of a reset is not.
