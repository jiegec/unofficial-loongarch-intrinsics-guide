# Buffer Objects

A buffer object (BO) is the unit of GPU memory allocation. It has a size, a
memory domain and a handle; it becomes addressable by the GPU only after a range
of it is mapped into the GPU virtual address space.

## Domains

| Domain | Value | Meaning |
|--------|------:|---------|
| `LOONGGPU_GEM_DOMAIN_CPU` | `0x1` | System memory that is not directly GPU accessible. |
| `LOONGGPU_GEM_DOMAIN_GTT` | `0x2` | GPU-accessible system memory. |
| `LOONGGPU_GEM_DOMAIN_VRAM` | `0x4` | Local video memory. |
| `LOONGGPU_GEM_DOMAIN_GDS` | `0x8` | Global on-chip data storage. |
| `LOONGGPU_GEM_DOMAIN_GWS` | `0x10` | Global wave synchronization. |
| `LOONGGPU_GEM_DOMAIN_OA` | `0x20` | Ordered append. |
| `LOONGGPU_GEM_DOMAIN_DOORBELL` | `0x40` | Doorbell memory; rejected by the current kernel. |

A BO is created in one or more domains. The kernel may migrate it between
domains; a resident buffer is placed before the submission that uses it.

## Creation flags

| Flag | Value | Meaning |
|------|------:|---------|
| `CPU_ACCESS_REQUIRED` | `1 << 0` | The BO will be accessed by the CPU. |
| `NO_CPU_ACCESS` | `1 << 1` | The VRAM BO is not CPU visible. |
| `CPU_GTT_USWC` | `1 << 2` | Use write-combined GTT memory. |
| `VRAM_CLEARED` | `1 << 3` | The VRAM BO is cleared. |
| `SHADOW` | `1 << 4` | Keep a GTT shadow. |
| `VRAM_CONTIGUOUS` | `1 << 5` | Allocate contiguous VRAM. |
| `VM_ALWAYS_VALID` | `1 << 6` | The BO is always valid in the VM. |
| `EXPLICIT_SYNC` | `1 << 7` | Sharing is explicitly synchronized. |
| `MQD_GFX9` | `1 << 8` | Legacy MQD allocation flag. |
| `COMPRESSED_MASK` | `0x7 << 9` | Compression mode. |
| `PREEMPTIBLE` | `1 << 12` | Preemptible allocation. |
| `DISCARDABLE` | `1 << 13` | Discardable allocation. |

The current kernel accepts a subset of these flags; `SHADOW`,
`VRAM_CONTIGUOUS`, `MQD_GFX9` and `PREEMPTIBLE` are defined by the interface but
are not accepted by the current allocation path. A caller SHOULD treat a
rejected flag as an error rather than assume it was applied.

The requested size is rounded up to the page size. A size of zero is rejected.
The alignment is rounded up to at least the page size.

## CPU mapping

`GEM_MMAP` returns an offset, not a pointer. A client maps the BO with:

```c
void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
```

The mapping length is the BO's size rounded to the page size. A BO created with
`NO_CPU_ACCESS` cannot be mapped.

## GPU mapping

`GEM_VA` maps a range of a BO into a GPU virtual address. The mapping is
page-granular and aligned. A range can be replaced or unmapped while no
submission that reads it is in flight; a client MUST keep the range mapped until
the fence of every submission that uses it has signalled.

The VA flags are listed in [User-space interface](uapi.md#buffer-objects). Only
the readable and writable flags reach the page-table entry; the executable flag
is required for an indirect buffer at the interface level but is not represented
in the PTE by the current kernel.

## Import, export and user pointers

A BO can be exported as a DMA-BUF file descriptor and imported by another
process. The importing process receives a handle and maps it like a locally
created BO.

`GEM_USERPTR` registers a user virtual address as a BO. The flags are:

| Flag | Value | Meaning |
|------|------:|---------|
| `READONLY` | `1 << 0` | The range is read-only. |
| `ANONONLY` | `1 << 1` | Only anonymous memory is accepted. |
| `VALIDATE` | `1 << 2` | Validate the range before use. |
| `REGISTER` | `1 << 3` | Register the range with the MMU notifier. |

The interface is not a stable substitute for an imported DMA-BUF: the
underlying mapping can be invalidated while a submission is in flight.

> UNRESOLVED: the exact behaviour of `GEM_USERPTR` when the user mapping is
> unmapped during a submission, and whether the kernel waits for the submission
> before releasing the range.

## Waiting and metadata

`GEM_WAIT_IDLE` waits until a BO is idle. The timeout is an absolute
`CLOCK_MONOTONIC` deadline in nanoseconds, or `~0ull` for no deadline. The output
reports whether the BO is idle and the domain it is currently in.

`GEM_METADATA` gets or sets tiling metadata. The metadata word packs:

| Field | Bits | Meaning |
|-------|------|---------|
| `ARRAY_MODE` | `[3:0]` | Array mode. |
| `PIPE_CONFIG` | `[8:4]` | Pipe configuration. |
| `TILE_SPLIT` | `[11:9]` | Tile split. |
| `MICRO_TILE_MODE` | `[14:12]` | Micro-tile mode. |
| `BANK_WIDTH` | `[16:15]` | Bank width. |
| `BANK_HEIGHT` | `[18:17]` | Bank height. |
| `MACRO_TILE_ASPECT` | `[20:19]` | Macro-tile aspect. |
| `NUM_BANKS` | `[22:21]` | Number of banks. |

An alternative encoding places a swizzle mode in `[4:0]`. The metadata is
device-dependent; a driver MUST NOT infer a layout from a metadata word without
the matching surface description.

## Residency lists

A submission names its resident buffers through a buffer list. The list is
created, updated or destroyed by `BO_LIST`:

| Operation | Value | Meaning |
|-----------|------:|---------|
| `CREATE` | 0 | Create a list; the output is the list handle. |
| `DESTROY` | 1 | Destroy a list. |
| `UPDATE` | 2 | Replace the list's entries. |

Each entry is a BO handle and a priority. Duplicate handles are collapsed. The
kernel rejects a list that does not fit its internal limit rather than splitting
the submission; a driver MUST split its own work.

## Lifecycle

A typical lifetime is:

1. create the BO with `GEM_CREATE`;
2. map a range with `GEM_VA`;
3. put the BO in a residency list and submit work;
4. wait for the submission's fence; the BO is now readable or writable by the
   CPU according to its mapping;
5. unmap the range with `GEM_VA` when no submission can still access it;
6. close the handle.

A BO handle can be closed while a submission still references it if the
submission's residency list holds its own reference. The memory is freed when
the last reference is dropped.

## Open questions

- The exact meaning of the tiling metadata fields for a tiled surface.
- Whether `GEM_USERPTR` ranges are pinned for the whole submission.
- The interaction between `GEM_WAIT_IDLE` and a BO that is also mapped by the
  CPU.
