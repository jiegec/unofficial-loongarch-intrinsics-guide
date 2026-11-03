# User-Space Interface

The LG200 is programmed through a DRM render node. This chapter defines that
surface: the objects a user-mode driver creates, what each ioctl validates, how a
submission is described, how completion is queried, how a compute queue reaches
the firmware, and what device information the interface publishes. Ioctl numbers,
argument structures, field widths and flag values are the ABI; a rejection
described as current behaviour is host policy that a later kernel may widen.

| Ioctl | ID | Argument | Privilege |
|-------|----|----------|-----------|
| `DRM_IOCTL_LOONGGPU_GEM_CREATE` | `0x00` | `union drm_loonggpu_gem_create` | render node |
| `DRM_IOCTL_LOONGGPU_GEM_MMAP` | `0x01` | `union drm_loonggpu_gem_mmap` | render node |
| `DRM_IOCTL_LOONGGPU_CTX` | `0x02` | `union drm_loonggpu_ctx` | render node |
| `DRM_IOCTL_LOONGGPU_BO_LIST` | `0x03` | `union drm_loonggpu_bo_list` | render node |
| `DRM_IOCTL_LOONGGPU_CS` | `0x04` | `union drm_loonggpu_cs` | render node |
| `DRM_IOCTL_LOONGGPU_INFO` | `0x05` | `struct drm_loonggpu_info` | render node |
| `DRM_IOCTL_LOONGGPU_GEM_METADATA` | `0x06` | `struct drm_loonggpu_gem_metadata` | render node |
| `DRM_IOCTL_LOONGGPU_GEM_WAIT_IDLE` | `0x07` | `union drm_loonggpu_gem_wait_idle` | render node |
| `DRM_IOCTL_LOONGGPU_GEM_VA` | `0x08` | `struct drm_loonggpu_gem_va` | render node |
| `DRM_IOCTL_LOONGGPU_WAIT_CS` | `0x09` | `union drm_loonggpu_wait_cs` | render node |
| `DRM_IOCTL_LOONGGPU_GEM_OP` | `0x10` | `struct drm_loonggpu_gem_op` | render node |
| `DRM_IOCTL_LOONGGPU_GEM_USERPTR` | `0x11` | `struct drm_loonggpu_gem_userptr` | render node |
| `DRM_IOCTL_LOONGGPU_WAIT_FENCES` | `0x12` | `union drm_loonggpu_wait_fences` | render node |
| `DRM_IOCTL_LOONGGPU_VM` | `0x13` | `union drm_loonggpu_vm` | render node |
| `DRM_IOCTL_LOONGGPU_FENCE_TO_HANDLE` | `0x14` | `union drm_loonggpu_fence_to_handle` | render node |
| `DRM_IOCTL_LOONGGPU_SCHED` | `0x15` | `union drm_loonggpu_sched` | master |
| `DRM_IOCTL_LOONGGPU_HWSEMA_OP` | `0x16` | `struct drm_loonggpu_hw_sema` | render node |
| `DRM_IOCTL_LOONGGPU_USERQ` | `0x1b` | `union drm_loonggpu_userq` | render node |
| `DRM_IOCTL_LOONGGPU_CTX_STATE3` | `0x1c` | `struct drm_loonggpu_ctx_state3` | render node |
| `DRM_IOCTL_LOONGGPU_FENCE_STATUS` | `0x1d` | `union drm_loonggpu_fence_status` | render node |

Command numbers `0x17` through `0x1a` and `0x20` through `0x22` are display
ioctls and are outside this chapter. The whole argument is copied in before any
check runs, so a field an operation does not use is still part of the structure.

## Device and context

A render node gives one file description, which owns one GPU virtual address space,
a table of contexts, a table of residency lists, at most one user compute queue and
an optional reserved VMID. A context is a scheduling entity per ring plus the
per-ring history that answers completion queries.

`DRM_IOCTL_LOONGGPU_CTX` takes `struct drm_loonggpu_ctx_in`: `op`, `flags`,
`ctx_id` and a signed `priority`. No bit of `flags` is defined and the current
kernel does not inspect it. `ctx_id` is an output of `ALLOC_CTX` (`1`) and an
input to `FREE_CTX` (`2`), `QUERY_STATE` (`3`) and `QUERY_STATE2` (`4`),
whose names are all prefixed `LOONGGPU_CTX_OP_`. Identifiers are allocated from 1
upwards and are never 0, and freeing one that is not live is refused.

The priority is one of six values, not a continuous scale: `-2048`
`LOONGGPU_CTX_PRIORITY_UNSET`, the sentinel for "no override" rather than a level
to create a context at; `-1023` `_VERY_LOW`; `-512` `_LOW`; `0`
`_NORMAL`; `512` `_HIGH`; and `1023` `_VERY_HIGH`. A priority above normal
is refused with `-EACCES` unless the caller holds `CAP_SYS_NICE` or the file
description is the DRM master. A value matching none of the six is not an error on
allocation, which creates the context at normal priority, while the current kernel
refuses to allocate at `LOONGGPU_CTX_PRIORITY_UNSET`.

`QUERY_STATE` reports whether a reset happened since the previous call of the same
operation on that context, and consuming the answer clears it: `0` means no reset
and `3` an unknown cause. Values `1` and `2` are defined as guilty and innocent
and are never returned by the current kernel.

`QUERY_STATE2` returns a 64-bit flag word and a reset count; the flags are sticky
for the life of the context: bit 0 `LOONGGPU_CTX_QUERY2_FLAGS_RESET` (a reset
happened after creation), bit 1 `_VRAMLOST`, bit 2 `_GUILTY` (a submission of
this context caused a hang), bit 3 `_DEVICE_LOST` (the device takes no new work)
and bit 4 `_RECOVERY_PENDING` (results from the old epoch stay provisional).
`hangs` is diagnostic and MUST NOT be used as a completion sequence.

`DRM_IOCTL_LOONGGPU_CTX_STATE3` adds the epoch identities. Its `flags` is a
32-bit copy of the `QUERY2` bits, `admission_state` is `0` open, `1` closed,
`2` restoring or `3` offline, and `quiesced_domains` carries `_DOMAIN_GFX`
(`1 << 0`), `_XDMA` (`1 << 1`) and `_CPIPE` (`1 << 2`) but is always 0: the
kernel has no per-domain evidence and answers zero rather than guess. A completion
carries the epoch it was admitted in, and a client MUST NOT treat a completion from
an older epoch as evidence that the device executed the work in its current state.

`DRM_IOCTL_LOONGGPU_SCHED` takes `{ op, fd, priority, flags }`, requires master,
MUST see `flags` as 0, and applies a priority to every context of the process
named by `fd` for `SCHED_OP_PROCESS_PRIORITY_OVERRIDE` (`1`). Unlike allocation,
an unknown priority is refused here.

`DRM_IOCTL_LOONGGPU_VM` reserves or unreserves one VMID for the file's VM
(`RESERVE_VMID` `1`, `UNRESERVE_VMID` `2`); its output flag word is reserved
and the ioctl writes nothing back. Unreserving is refused with `-EBUSY` while the
file owns a user compute queue, which needs the reservation to keep executing in a
VMID whose page directory this VM has bound.

## Buffer objects

A buffer object is an allocation with a handle, and it becomes addressable by the
GPU only when a range of it is mapped with `GEM_VA`. Domains, creation flags and
migration are described in [buffer objects](buffer-objects.md); this section fixes
the arguments and the checks.

`DRM_IOCTL_LOONGGPU_GEM_CREATE` takes `struct drm_loonggpu_gem_create_in` —
`bo_size`, `alignment`, `domains`, `domain_flags` — and returns a 32-bit
`out.handle`. The size is rounded up to the system page size; a size that is zero
after rounding, or whose round-up overflows, is refused, and an alignment below the
page size is raised to it. `domains` MUST be a subset of
`LOONGGPU_GEM_DOMAIN_MASK`, and the doorbell domain (`0x40`) is defined and
refused.

The accepted flag mask is `CPU_ACCESS_REQUIRED` (`1 << 0`), `NO_CPU_ACCESS`
(`1 << 1`), `CPU_GTT_USWC` (`1 << 2`), `VRAM_CLEARED` (`1 << 3`),
`VM_ALWAYS_VALID` (`1 << 6`), `EXPLICIT_SYNC` (`1 << 7`),
`COMPRESSED_MASK` (`0x7 << 9`) and `DISCARDABLE` (`1 << 13`). Any other bit
is refused, so `SHADOW`, `VRAM_CONTIGUOUS`, `MQD_GFX9` and `PREEMPTIBLE` are
defined and not accepted. A non-zero compression mode also requires the VRAM domain
alone and a device that supports compression.

`DRM_IOCTL_LOONGGPU_GEM_MMAP` returns `out.addr_ptr`, the offset to pass to
`mmap()`: an offset, not a pointer. A buffer registered from a user pointer, or
created with `NO_CPU_ACCESS`, is refused with `-EPERM`.

`DRM_IOCTL_LOONGGPU_GEM_WAIT_IDLE` takes `{ handle, flags, timeout }` and
returns `{ status, domain }`. `timeout` is an absolute `CLOCK_MONOTONIC`
deadline in nanoseconds, `~0ull` meaning no deadline; `status` is 0 when the
buffer is idle and 1 when the deadline passed first, and `domain` is written as 0.
It waits for the buffer's own work, not for a submission.

`DRM_IOCTL_LOONGGPU_GEM_VA` takes `struct drm_loonggpu_gem_va` — `handle`,
`operation`, `flags`, `va_address`, `offset_in_bo`, `map_size`.
`LOONGGPU_VA_OP_MAP` (`1`) adds a mapping, `_REPLACE` (`4`) replaces one,
`_UNMAP` (`2`) removes the one that starts at `va_address`, and `_CLEAR`
(`3`) removes every mapping in the range and needs no handle.

| Flag | Value | Meaning |
|------|------:|---------|
| `LOONGGPU_VM_DELAY_UPDATE` | `1 << 0` | Record the mapping; publish the page directory on the next submission. |
| `LOONGGPU_VM_PAGE_READABLE` | `1 << 1` | Sets the present bit in the page-table entry. |
| `LOONGGPU_VM_PAGE_WRITEABLE` | `1 << 2` | Sets the write bit. |
| `LOONGGPU_VM_PAGE_EXECUTABLE` | `1 << 3` | Defined; changes no page-table bit. |
| `LOONGGPU_VM_PAGE_PRT` | `1 << 4` | Map the shared partial-residency object instead of `handle`. |
| `LOONGGPU_VM_MTYPE_MASK` | `0xf << 5` | Memory type: `DEFAULT` `0 << 5`, `NC` `1 << 5`, `WC` `2 << 5`, `CC` `3 << 5`, `UC` `4 << 5`. |

`MAP`, `REPLACE` and `CLEAR` need a non-zero `map_size`, and `va_address`
and `map_size` MUST be multiples of the GPU page size; `UNMAP` is checked for
address alignment only. For `MAP` and `REPLACE`, `offset_in_bo` MUST be page
aligned and `offset_in_bo + map_size` MUST fit inside the buffer. The address MUST
be at or above a reserved floor, 1 MiB by default, and outside the hole that spans
`2^47` to `0xffff_8000_0000_0000`; it is then reduced to its low 48 bits, and is
checked against the floor again. `va_address + map_size` MUST not overflow and MUST
stay below the device's virtual address limit. See
[memory system](../architecture/memory-system.md) for why the low 1 MiB is
excluded.

Only `READABLE` and `WRITEABLE` reach the page-table entry: this format has no
execute bit, and no field that the memory-type bits feed. A mapping a submission
will read through therefore MUST carry `READABLE`; `EXECUTABLE` is neither
required nor representable. The page tables are written before `MAP` and
`REPLACE` return unless `DELAY_UPDATE` is set, and a failed write rolls the
mapping back. A range MUST stay mapped until every submission that names it has
retired.

`DRM_IOCTL_LOONGGPU_GEM_OP` takes `{ handle, op, value }`:
`LOONGGPU_GEM_OP_GET_GEM_CREATE_INFO` (`0`) writes a `struct
drm_loonggpu_gem_create_in` to the user address in `value`, and
`SET_PLACEMENT` (`1`) takes a domain mask restricted to `CPU`, `GTT` and
`VRAM`. `DRM_IOCTL_LOONGGPU_GEM_METADATA` takes `{ handle, op, data }` with
`SET_METADATA` (`1`) and `GET_METADATA` (`2`); `data` holds `flags`,
`tiling_info`, `data_size_bytes` and a 64-dword array, and setting more than
those 256 bytes is refused.

`DRM_IOCTL_LOONGGPU_GEM_USERPTR` takes `{ addr, size, flags, handle }`, with
`addr` and `size` page aligned. `READONLY` (`1 << 0`), `ANONONLY`
(`1 << 1`), `VALIDATE` (`1 << 2`) and `REGISTER` (`1 << 3`) are defined
and any other bit is refused. A registration that may be written through MUST set
`REGISTER`, because writing needs the invalidation notifier; without either
`READONLY` or `REGISTER` it is refused with `-EACCES`. The interface is
explicitly unreliable, so a client MUST have a fallback that does not register its
own memory.

## Submission

A submission names one context, an optional residency list and a chunk array.

`DRM_IOCTL_LOONGGPU_BO_LIST` takes `{ operation, list_handle, bo_number,
bo_info_size, bo_info_ptr }` with `CREATE` (`0`), `DESTROY` (`1`) and
`UPDATE` (`2`); `list_handle` is the existing list for destroy and update and
0 for create. An entry is `struct drm_loonggpu_bo_list_entry` — `bo_handle` and
`bo_priority` — and `bo_info_size` is its stride. `bo_priority` is clamped to
32 and only orders buffers during validation, and the output `list_handle` is 0
after a successful destroy.

`DRM_IOCTL_LOONGGPU_CS` takes `struct drm_loonggpu_cs_in` — `ctx_id`,
`bo_list_handle`, `num_chunks`, `chunks`. `num_chunks` is 1 through 4096.
`chunks` is the user address of an array of 64-bit user addresses; each element
points at a `struct drm_loonggpu_cs_chunk` — `chunk_id`, `length_dw`,
`chunk_data` — where `length_dw` is the payload length in 32-bit words and
`chunk_data` is the user address of the payload. A payload longer than `1 << 20`
words (4 MiB) is refused, and so is a chunk whose payload is not exactly the size
of its record where the record is fixed.

| Chunk | ID | Payload | Records |
|-------|----|---------|--------:|
| `LOONGGPU_CHUNK_ID_IB` | `0x01` | `struct drm_loonggpu_cs_chunk_ib` | exactly 1 |
| `LOONGGPU_CHUNK_ID_FENCE` | `0x02` | `struct drm_loonggpu_cs_chunk_fence` | exactly 1 |
| `LOONGGPU_CHUNK_ID_DEPENDENCIES` | `0x03` | Array of `struct drm_loonggpu_cs_chunk_dep` | 1 or more |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_IN` | `0x04` | Array of `struct drm_loonggpu_cs_chunk_sem` | 1 or more |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_OUT` | `0x05` | Array of `struct drm_loonggpu_cs_chunk_sem` | 1 or more |
| `LOONGGPU_CHUNK_ID_BO_HANDLES` | `0x06` | `struct drm_loonggpu_bo_list_in` and its entries | 1 header |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_TIMELINE_WAIT` | `0x08` | Array of `struct drm_loonggpu_cs_chunk_syncobj` | 1 or more |
| `LOONGGPU_CHUNK_ID_SYNCOBJ_TIMELINE_SIGNAL` | `0x09` | Array of `struct drm_loonggpu_cs_chunk_syncobj` | 1 or more |

Chunk ID `0x07` is reserved and refused. Every chunk except `IB` may appear at
most once, so a repeated fence, dependency, sync-object or residency chunk is
rejected before any payload is used. `bo_list_handle` and a `BO_HANDLES` chunk
are mutually exclusive; with neither, the submission has an empty residency set and
can name no buffer. The `FENCE` chunk names a buffer that MUST be exactly one page
and an offset that leaves room for a 64-bit value; the engine writes the submission's
sequence number there as the work completes.

The IB chunk is `struct drm_loonggpu_cs_chunk_ib` — `flags`, `va_start`,
`ib_bytes`, `ip_type`, `ip_instance`, `ring`:

| Flag | Value | Meaning |
|------|------:|---------|
| `LOONGGPU_IB_FLAG_CE` | `1 << 0` | Submit to the copy engine. |
| `LOONGGPU_IB_FLAG_PREAMBLE` | `1 << 1` | The buffer may be dropped if no context switch is needed. |
| `LOONGGPU_IB_FLAG_PREEMPT` | `1 << 2` | Defined; refused by the current kernel. |
| `LOONGGPU_IB_FLAG_TC_WB_NOT_INVALIDATE` | `1 << 3` | Defined; refused by the current kernel. |

`ib_bytes` MUST be non-zero and a multiple of 4, and at most `0x03ff_fffc`
bytes, which is `0x00ff_ffff` words. `va_start` MUST be 4-byte aligned. The
whole range MUST lie inside a single present `GEM_VA` mapping, the buffer behind
it MUST be in this submission's residency set, and the range MUST fit inside that
buffer. Admission tests presence, not permission: whether the mapping is writable
is not inspected.

The engine is selected by `ip_type`, `ip_instance` and `ring` together.
`LOONGGPU_HW_IP_GFX` (`0`) is the graphics ring, `LOONGGPU_HW_IP_DMA` (`2`)
the transfer ring and `LOONGGPU_HW_IP_BPIPE` (`8`) the blit ring;
`LOONGGPU_HW_IP_COMPUTE` (`1`) is refused with `-ENODEV`, because compute is
reached through `USERQ` and not through `CS`, and `LOONGGPU_HW_IP_DPIPE`
(`5`) and `_EPIPE` (`6`) are defined with no engine on this generation.
`ip_instance` MUST be 0, because `LOONGGPU_HW_IP_INSTANCE_MAX_COUNT` is 1, and
`ring` MUST be below the engine's ring count, which is 1 for every engine of this
generation.

Every IB chunk of one submission MUST resolve to the same ring. The output is
`out.handle`, a 64-bit sequence number, unique per context and ring and starting
at 1, which is how `WAIT_CS`, `FENCE_STATUS`, `FENCE_TO_HANDLE` and a
dependency record name this submission.

A submission is refused with `-EBUSY` when the accelerator is not working, with
`-EAGAIN` while a fault-storm window is open for this client or while the compute
pipe is quarantined and its recovery has not finished, and with `-ECANCELED` when
the context is marked guilty or its local memory was lost after creation.

## Waiting and completion

`DRM_IOCTL_LOONGGPU_WAIT_CS` takes `struct drm_loonggpu_wait_cs_in` —
`handle`, `timeout`, `ip_type`, `ip_instance`, `ring`, `ctx_id` — and
returns a 64-bit `status`. `handle` is a sequence from `CS`, `0` for none, or
`~0ull` for the newest issued; `timeout` is an absolute `CLOCK_MONOTONIC`
deadline in nanoseconds, `~0ull` meaning no deadline. `status` is 0 when the
submission completed before the deadline, or when there was nothing to wait for,
and 1 when the deadline passed first. A handle of 0 names no submission and reports
completion immediately, including on a ring that has never been submitted to. If
the submission finished with an error, the ioctl returns that error instead of a
status.

`DRM_IOCTL_LOONGGPU_WAIT_FENCES` takes `{ fences, fence_count, wait_all,
timeout_ns }` and returns `{ status, first_signaled }`. `fences` is the user
address of an array of `struct drm_loonggpu_fence` — `ctx_id`, `ip_type`,
`ip_instance`, `ring`, `seq_no` — and `fence_count` MUST be at least 1. A
non-zero `wait_all` waits for every fence, zero waits for any one. `status` is 1
when the wait was satisfied and 0 when the deadline passed; `first_signaled` is
the index that satisfied an any-of wait and 0 for an all-of wait. A signalled fence
that carries an error makes the ioctl return that error.

`DRM_IOCTL_LOONGGPU_FENCE_STATUS` answers without blocking whether one submission
has retired. Its input is `{ ctx_id, ip_type, ip_instance, ring, seq_no, flags }`;
`flags` MUST be 0, and a non-zero value is refused rather than ignored because a
caller that asked for semantics this kernel does not implement must not receive an
answer that looks as if it did. Its output is `{ flags, error, seq_epoch,
current_epoch, confirmed }`: `error` is 0 or the execution error of a failed
submission, `seq_epoch` the epoch the sequence was admitted in,
`current_epoch` the device's epoch now, and `confirmed` a completion watermark
for this context and ring.

| Bit | Name | Meaning |
|-----|------|---------|
| 0 | `LOONGGPU_FENCE_RETIRED` | The submission can no longer access its buffers. |
| 1 | `LOONGGPU_FENCE_ERROR` | The submission failed; this does not prove retirement. |
| 2 | `LOONGGPU_FENCE_EPOCH_STALE` | Admitted before the current reset epoch. |
| 3 | `LOONGGPU_FENCE_VRAM_LOST` | Local memory was lost since admission. |
| 4 | `LOONGGPU_FENCE_HISTORY_EVICTED` | The sequence aged out of the per-context history. |
| 5 | `LOONGGPU_FENCE_DEVICE_LOST` | The device is not executing; nothing new will complete. |

`RETIRED` is the only fact that authorises handing the memory a submission named
to another user. It is set only when this submission's own completion was reported
by the pipe without an error, and never for a submission that named no buffer.
`ERROR` is set when the submission finished with an error, including a forced
completion after a failed wait, and never accompanies `RETIRED`: a reset stops the
engine but leaves the data it wrote behind. `EPOCH_STALE` is advisory: the
completion, if any, belongs to a device state that no longer exists.
`HISTORY_EVICTED` means the sequence is older than the bounded history the kernel
retains, 32 submissions per ring by default; the answer then comes from watermarks,
and `RETIRED` is reported only if a watermark proves the ring drained past that
sequence. `confirmed` is not a count of submitted jobs and MUST NOT override
`ERROR` or the absence of `RETIRED`. Retirement does not make a buffer's contents
visible to the CPU and says nothing about other users of a shared buffer; CPU access
still needs `GEM_WAIT_IDLE`.

`DRM_IOCTL_LOONGGPU_FENCE_TO_HANDLE` turns a fence into a shareable object: its
input is a `struct drm_loonggpu_fence` followed by `{ what, pad }`, and its
output `handle` is a sync-object handle in this file description for
`LOONGGPU_FENCE_TO_HANDLE_GET_SYNCOBJ` (`0`), a sync-object file descriptor for
`_GET_SYNCOBJ_FD` (`1`), or a sync-file file descriptor for
`_GET_SYNC_FILE_FD` (`2`).

`DRM_IOCTL_LOONGGPU_HWSEMA_OP` takes `{ id, ctx_id, ops }`.
`LOONGGPU_HW_SEMA_GET` (`1`) claims a free semaphore and returns its index in
`id`, or `-ENODATA` with `id` set to `~0ull` when none is free; `_PUT`
(`2`) releases one and refuses another VM's semaphore with `-EPERM`. Ownership
belongs to the VM that claimed it, and `ctx_id` authorises nothing.

## User compute queues

A user-mode driver owns the compute ring, maps it in its own address space, and the
kernel's only jobs are to validate the geometry, hand the queue to the firmware and
forward doorbells. A compute queue therefore exists only where the compute firmware
interface is present and the part is LG200 or LG210; elsewhere `USERQ` returns
`-ENODEV`. The command stream inside the ring is described in
[compute submission](compute-submission.md).

`DRM_IOCTL_LOONGGPU_USERQ` takes `struct drm_loonggpu_userq_in` — `op`,
`queue_id`, `ip_type`, `queue_va`, `queue_size`, `wptr`. `ip_type` MUST be
`LOONGGPU_HW_IP_COMPUTE`. `CREATE` is `1`, `FREE` `2` and `DOORBELL`
`3`; `queue_id` is an output of create and an input to the others.

`CREATE` requires `queue_size` to be a power of two between
`LOONGGPU_USERQ_MIN_SIZE` (4096) and `LOONGGPU_USERQ_MAX_SIZE` (`1 << 20`),
`queue_va` to be 64-byte aligned and at least 64, and the file's VM to have a
PASID, which it gets when it is first mapped for GPU use. The 64 bytes before
`queue_va` and the whole ring MUST lie inside one `GEM_VA` mapping, and the
queue takes its own reference to that buffer and mapping, so dropping the GEM
handle does not tear it down. A file description owns at most one compute queue, so
a second `CREATE` returns `-EBUSY`. The output is `out.queue_id`, the queue's
slot number in this file, starting at 0: slots are per file and the firmware keeps
two clients' slot 0 apart by PASID.

The ring is preceded by a 64-byte index block holding two 64-bit byte positions, 8
bytes apart: the write index at `queue_va - 64`, which the client stores and from
which the firmware learns where the next packet starts, and the read index at
`queue_va - 56`, which the firmware writes back as it consumes packets. Both count
bytes from the start of the ring, are ordinary client memory, and MUST stay mapped
for the lifetime of the queue. The ring itself starts at byte 0 of `queue_va` and
is `queue_size` bytes long, so the index block is never part of the stream.

The client writes the write index itself; `DOORBELL` only tells the firmware to
look at it again. The kernel checks that the `wptr` argument is 64-byte aligned and
smaller than `queue_size`, records it for diagnostics, and copies nothing into the
index block: the argument and the stored position are independent, so that check
constrains what the caller passed, not what the device reads. The kernel never reads
the command stream, so a malformed packet can only damage the process that wrote it.

Completion is not a fence. No sequence number is issued for a user queue and no
ioctl waits on one; the client establishes completion from the read index advancing
and from the dispatch completion signal the stream itself programs. A queue
whose write index stays ahead of the read index without progress is treated as
stalled: the kernel retracts that client's compute queues by PASID, and only if that
fails does it fall back to device recovery, so a client MUST NOT assume a stalled
queue will simply block forever.

`FREE` retracts the queue at the firmware and then releases the slot, the ring
reference and, for the last queue, the reserved VMID. Because the firmware's
teardown packet is filtered by PASID, freeing a queue releases that client's compute
queue as a whole, which is also why a second queue per file is refused.

## Device information

`DRM_IOCTL_LOONGGPU_INFO` takes `struct drm_loonggpu_info` — `return_pointer`,
`return_size`, `query` and an 8-byte query-specific union. Both `return_pointer`
and `return_size` MUST be non-zero, and the kernel writes at most the smaller of
`return_size` and the size of the answer, so a short buffer receives a prefix and
success. An unassigned selector is refused.

| Selector | Value | Answer |
|----------|------:|--------|
| `LOONGGPU_INFO_ACCEL_WORKING` | `0x00` | 32-bit flag: the accelerator is usable. |
| `LOONGGPU_INFO_HW_IP_INFO` | `0x02` | `struct drm_loonggpu_info_hw_ip`. |
| `LOONGGPU_INFO_HW_IP_COUNT` | `0x03` | 32-bit instance count, 0 or 1. |
| `LOONGGPU_INFO_TIMESTAMP` | `0x05` | 64-bit GPU clock counter. |
| `LOONGGPU_INFO_FW_VERSION` | `0x0e` | `struct drm_loonggpu_info_firmware`. |
| `LOONGGPU_INFO_VRAM_GTT` | `0x14` | `struct drm_loonggpu_info_vram_gtt`. |
| `LOONGGPU_INFO_DEV_INFO` | `0x16` | `struct drm_loonggpu_info_device`. |
| `LOONGGPU_INFO_MEMORY` | `0x19` | `struct drm_loonggpu_memory_info`. |
| `LOONGGPU_INFO_VRAM_LOST_COUNTER` | `0x1f` | 32-bit counter. |
| `LOONGGPU_INFO_MAX_IBS` | `0x20` | 11 32-bit values, one per `LOONGGPU_HW_IP_*`. |

`LOONGGPU_INFO_NUM_BYTES_MOVED` (`0x0f`), `_VRAM_USAGE` (`0x10`),
`_GTT_USAGE` (`0x11`), `_VIS_VRAM_USAGE` (`0x17`), `_NUM_EVICTIONS`
(`0x18`) and `_NUM_VRAM_CPU_PAGE_FAULTS` (`0x1e`) are 64-bit counters.
`LOONGGPU_INFO_GDS_CONFIG` (`0x13`) is refused with `-ENODATA`.

`LOONGGPU_INFO_DEV_INFO` is what an address manager needs. It reports
`virtual_address_offset`, the first client virtual address, with the low 1 MiB
reserved; `virtual_address_max`, the end of the low client range;
`high_va_offset` and `high_va_max`, the range above the hole, present only when
the address space is large enough to need it; `virtual_address_alignment`, the
required alignment of a `GEM_VA` address; `pte_fragment_size`, the granularity
of a page-table update; and `gart_page_size`, the page size of the GART aperture.
It also reports the device identity, the shader-engine and render-backend counts,
the wavefront size, the visible VGPR count, the VRAM type and width, and
`ids_flags`, whose `FUSION` bit (`0x1`) marks an integrated device.

`LOONGGPU_INFO_HW_IP_INFO` returns `{ hw_ip_version_major, hw_ip_version_minor,
capabilities_flags, ib_start_alignment, ib_size_alignment, available_rings }` for
one engine class and instance 0. `available_rings` is a bitmask of rings that are
up. The alignment fields are per engine: 32 bytes and 8 bytes for the graphics, blit
and display pipes, 256 bytes and 8 bytes for transfer, and 64 bytes for both fields
on compute. A compute entry reports the queue slots sealed for this interface as its
ring bits. `LOONGGPU_INFO_HW_IP_COUNT` answers 1 for compute only while that
partition is sealed, ready and not quarantined.

## Open questions

- The memory type a mapping actually receives. The interface carries a memory-type
  field but no bit of it reaches the page-table entry, and the sources do not say
  which cache policy the hardware applies to a mapping created without one.
  UNRESOLVED: whether a compute stream can require a non-default policy.
- What the engine does with a write index that names a position past the end of the
  ring, that is, whether a stream may straddle byte `queue_size` and continue at
  byte 0. The kernel validates only the `wptr` argument and never reads the ring, so
  the rule the firmware applies to the stored byte position is not established.
  UNRESOLVED: a client MUST either keep every stream contiguous or know how the
  hardware wraps.
- The largest ring a user compute queue may use. The interface bounds it to 1 MiB;
  whether the firmware has a smaller limit is not established. UNRESOLVED: a driver
  that sizes to the interface limit may be refused by a firmware revision.
- The behaviour of a user-pointer buffer that the process unmaps while a submission
  naming it is in flight. A submission whose pages are stale is refused before it
  is queued, but nothing in the sources establishes what the engine does to a
  translation it has already taken. UNRESOLVED.
