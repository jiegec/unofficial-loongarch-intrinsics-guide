# User-Space Interface

The LG200 is driven through the `loonggpu` kernel driver and the
`libdrm_loonggpu` library. This chapter defines the ioctl surface: the objects a
user-mode driver creates, the commands it submits and the fences it waits on.
The kernel's internal implementation is outside this manual.

## Device and context

A client opens a render node and creates a context. The context carries the
priority, the reset state and the last submitted sequence number for each ring.

| Ioctl | ID | Purpose |
|-------|----|---------|
| `DRM_IOCTL_LOONGGPU_CTX` | `0x02` | Allocate, free or query a context. |
| `DRM_IOCTL_LOONGGPU_CTX_STATE3` | `0x1c` | Query the context's reset and admission epochs. |
| `DRM_IOCTL_LOONGGPU_SCHED` | `0x15` | Override a process priority; requires `DRM_MASTER`. |

`DRM_IOCTL_LOONGGPU_CTX` takes an operation, a context id and a priority:

| Operation | Value | Meaning |
|-----------|------:|---------|
| `LOONGGPU_CTX_OP_ALLOC_CTX` | 1 | Allocate a context; the output is the context id. |
| `LOONGGPU_CTX_OP_FREE_CTX` | 2 | Free a context. |
| `LOONGGPU_CTX_OP_QUERY_STATE` | 3 | Return the reset status. |
| `LOONGGPU_CTX_OP_QUERY_STATE2` | 4 | Return the reset status, hang count and flags. |

Priorities range from `-2048` to `1023`. A priority above `LOONGGPU_CTX_PRIORITY_NORMAL`
(0) requires `CAP_SYS_NICE` or `DRM_MASTER`.

The state query returns `LOONGGPU_CTX_QUERY2_FLAGS_*` bits:

| Flag | Meaning |
|------|---------|
| `RESET` | A reset occurred since the context was created. |
| `VRAMLOST` | VRAM was lost. |
| `GUILTY` | A job from this context caused a hang. |
| `DEVICE_LOST` | The device is not accepting new work. |
| `RECOVERY_PENDING` | A recovery coordinator is active; old-epoch results are provisional. |

The typed state query `CTX_STATE3` adds the reset epoch, the admission epoch and
the admission state (open, closed, restoring or offline). A client MUST NOT
treat a completion from an older epoch as proof that the new device state
executed the work.

## Buffer objects

A buffer object is created by `GEM_CREATE` and mapped into the GPU address space
by `GEM_VA`. A client may also import or export a buffer through a DMA-BUF
handle, map a user pointer, or query and set tiling metadata.

| Ioctl | ID | Purpose |
|-------|----|---------|
| `DRM_IOCTL_LOONGGPU_GEM_CREATE` | `0x00` | Allocate a buffer object. |
| `DRM_IOCTL_LOONGGPU_GEM_MMAP` | `0x01` | Return an mmap offset for CPU access. |
| `DRM_IOCTL_LOONGGPU_GEM_VA` | `0x08` | Map, unmap or replace a GPU virtual-address range. |
| `DRM_IOCTL_LOONGGPU_GEM_WAIT_IDLE` | `0x07` | Wait until a buffer is idle. |
| `DRM_IOCTL_LOONGGPU_GEM_OP` | `0x10` | Get creation information or set placement. |
| `DRM_IOCTL_LOONGGPU_GEM_METADATA` | `0x06` | Get or set tiling metadata. |
| `DRM_IOCTL_LOONGGPU_GEM_USERPTR` | `0x11` | Register a user pointer as a buffer object. |

`GEM_CREATE` takes the requested size, an alignment, a domain mask and a flag
mask. The size is rounded up to the page size and MUST be nonzero. The domains
and flags are described in [Buffer objects](buffer-objects.md).

`GEM_MMAP` returns an offset, not a pointer. A client passes that offset to
`mmap()`; the mapping length is the buffer's size, rounded to the page size.

`GEM_VA` takes an operation, a buffer handle, an offset inside the buffer, a GPU
virtual address and a size. The operation is one of `MAP` (1), `UNMAP` (2),
`CLEAR` (3) or `REPLACE` (4). The address range, the offset and the size MUST be
aligned to the mapping granularity. The flags select readability, writability,
executability, partial residency and a memory type.

| Flag | Value | Meaning |
|------|------:|---------|
| `LOONGGPU_VM_PAGE_READABLE` | `1 << 1` | Readable mapping. |
| `LOONGGPU_VM_PAGE_WRITEABLE` | `1 << 2` | Writable mapping. |
| `LOONGGPU_VM_PAGE_EXECUTABLE` | `1 << 3` | Executable mapping. |
| `LOONGGPU_VM_PAGE_PRT` | `1 << 4` | Partially resident. |
| `LOONGGPU_VM_MTYPE_*` | `0xf << 5` | Memory type: default, NC, WC, CC or UC. |

The current kernel represents only `READABLE` (as present) and `WRITEABLE` in the
page-table entry; the other flags do not change the PTE. See
[Memory system](../architecture/memory-system.md).

## Submission

A command submission is the `CS` ioctl. It names a context, a buffer list and an
array of chunks. Each chunk has a type, a length in dwords and a pointer to its
payload.

| Ioctl | ID | Purpose |
|-------|----|---------|
| `DRM_IOCTL_LOONGGPU_BO_LIST` | `0x03` | Create, destroy or update a residency list. |
| `DRM_IOCTL_LOONGGPU_CS` | `0x04` | Submit a command buffer. |
| `DRM_IOCTL_LOONGGPU_WAIT_CS` | `0x09` | Wait for one sequence number. |
| `DRM_IOCTL_LOONGGPU_WAIT_FENCES` | `0x12` | Wait for an array of fences. |
| `DRM_IOCTL_LOONGGPU_FENCE_STATUS` | `0x1d` | Query retirement, error and epoch state. |
| `DRM_IOCTL_LOONGGPU_FENCE_TO_HANDLE` | `0x14` | Convert a fence to a sync object or a file descriptor. |
| `DRM_IOCTL_LOONGGPU_USERQ` | `0x1b` | Create, free or ring a user compute queue. |
| `DRM_IOCTL_LOONGGPU_VM` | `0x13` | Reserve or unreserve a VMID. |
| `DRM_IOCTL_LOONGGPU_HWSEMA_OP` | `0x16` | Acquire or release a hardware semaphore. |
| `DRM_IOCTL_LOONGGPU_INFO` | `0x05` | Query device information, firmware and the timestamp. |

`CS` takes:

| Field | Meaning |
|-------|---------|
| `ctx_id` | The context that owns the submission. |
| `bo_list_handle` | The residency list, or zero for none. |
| `num_chunks` | Number of chunks; 1 through 4096. |
| `chunks` | Pointer to an array of `struct drm_loonggpu_cs_chunk`. |

The output is a 64-bit sequence number. It is the handle used by `WAIT_CS`, by
`FENCE_STATUS` and by a dependency chunk.

| Chunk | ID | Payload |
|-------|----|---------|
| `LOONGGPU_CHUNK_ID_IB` | `0x01` | `struct drm_loonggpu_cs_chunk_ib`: flags, `va_start`, `ib_bytes`, `ip_type`, `ip_instance`, `ring`. |
| `LOONGGPU_CHUNK_ID_FENCE` | `0x02` | `struct drm_loonggpu_cs_chunk_fence`: handle and offset. |
| `LOONGGPU_CHUNK_ID_DEPENDENCIES` | `0x03` | Array of `struct drm_loonggpu_cs_chunk_dep`. |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_IN` | `0x04` | Array of `struct drm_loonggpu_cs_chunk_syncobj` to wait on. |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_OUT` | `0x05` | Array of sync-object handles to signal. |
| `LOONGGPU_CHUNK_ID_BO_HANDLES` | `0x06` | Buffer handles used by the submission. |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_TIMELINE_WAIT` | `0x08` | Timeline wait: handle, flags, point. |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_TIMELINE_SIGNAL` | `0x09` | Timeline signal: handle, flags, point. |

The IB chunk's `flags` are `LOONGGPU_IB_FLAG_CE` (submit to the copy engine),
`PREAMBLE`, `PREEMPT` and `TC_WB_NOT_INVALIDATE`. The IB's GPU virtual address
MUST be mapped executable. The length is in bytes and MUST be a multiple of four.

A submission MUST target one ring: every IB chunk in one `CS` carries the same
`ip_type`, `ip_instance` and `ring`. The maximum chunk size is 4 MiB, and the
maximum IB length is `0x00ff_ffff` dwords. A fence buffer MUST be exactly one page.

## Waiting and completion

`WAIT_CS` takes a fence handle, an absolute `CLOCK_MONOTONIC` deadline in
nanoseconds (or `~0ull` for no deadline), and the engine identifiers. A handle of
zero means "nothing to wait for"; `~0ull` means "the latest sequence on this
ring". `WAIT_FENCES` takes an array of fences, a wait-all flag and an absolute
deadline.

`FENCE_STATUS` takes a context, engine, ring and sequence number (or `~0ull` for
the latest) and returns:

| Flag | Value | Meaning |
|------|------:|---------|
| `LOONGGPU_FENCE_RETIRED` | `1 << 0` | The submission can no longer access its buffers. |
| `LOONGGPU_FENCE_ERROR` | `1 << 1` | The submission failed; this does not prove retirement. |
| `LOONGGPU_FENCE_EPOCH_STALE` | `1 << 2` | Admitted before the current reset epoch. |
| `LOONGGPU_FENCE_VRAM_LOST` | `1 << 3` | VRAM was lost since admission. |
| `LOONGGPU_FENCE_HISTORY_EVICTED` | `1 << 4` | The sequence aged out of the history. |
| `LOONGGPU_FENCE_DEVICE_LOST` | `1 << 5` | The device is not executing. |

The query also returns the sequence's admission epoch, the device's current
epoch and a completion watermark. A watermark is not a count of submitted jobs
and does not override an error or the absence of retirement.

## User compute queues

`USERQ` creates, frees or rings a user compute queue. The operation is one of
`CREATE` (1), `FREE` (2) or `DOORBELL` (3).

| Field | Meaning |
|-------|---------|
| `queue_id` | Output on create; input on free and doorbell. |
| `ip_type` | `LOONGGPU_HW_IP_COMPUTE` for a compute queue. |
| `queue_va` | GPU virtual address of the ring. |
| `queue_size` | Ring size in bytes; a power of two from 4 KiB to 1 MiB. |
| `wptr` | New write pointer in bytes, for `DOORBELL`. |

The ring is preceded by a 64-byte index block. The write index is a 64-bit value
at `queue_va - 64`; the read index is at `queue_va - 56`. Both regions MUST stay
mapped for the lifetime of the queue. The ring and index block MUST be 64-byte
aligned.

A doorbell publishes a new write pointer. The kernel does not parse the ring.
The queue's completion is a fence, as for a graphics submission.

## Device information

`INFO` returns device properties, firmware versions, memory heaps, the hardware
IP table and the GPU timestamp. The fields a driver needs for address management
are the virtual-address offset and maximum, the virtual-address alignment, the
PTE fragment size and the GART page size.

## Open questions

- The exact set of creation flags accepted by the kernel for every domain.
- The behaviour of `GEM_USERPTR` when the underlying mapping is unmapped while a
  submission is in flight.
- Whether `WAIT_CS` and `WAIT_FENCES` share a single deadline interpretation on
  all engines.
- The legal range of the `ip_instance` and `ring` selectors for each engine.
