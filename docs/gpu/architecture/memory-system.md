# Memory System

This chapter describes how the LG200 addresses memory: the virtual address
space, the page tables, the VMIDs, the segment apertures and the ordering rules
that make writes visible.

![GPU virtual address space](../assets/fig-va.svg)

The figure shows the 1 MiB head, the single 1 TiB client range, the hole above
it and the two decode apertures outside the client walk.

## Address space

The client GPU virtual-address window on LG200 is **1 TiB (40 bits)**, not
the full 48-bit linear form. The kernel excludes the low 1 MiB with a per-address floor and publishes one
low client range; nothing is mapped in that head. On the current kernel the range ends at
`0x00ff_fff0_0000`; the end is reported by the device-information query, so a
driver MUST use the reported value rather than assume the maximum.

| Range | Use |
|-------|-----|
| `0x0000_0000_0000_0000` – `0x0000_0000_000f_ffff` | Excluded by the kernel as a floor; not mapped and not available to a client. |
| `0x0000_0000_0010_0000` – `0x0000_00ff_fff0_0000` | Client range; one root page directory per VMID. |
| `0x0000_0100_0000_0000` – `0x0000_7fff_ffff_ffff` | Above the 40-bit client walk; not mapped by a client page table. |
| `0x0000_8000_0000_0000` – `0xffff_7fff_ffff_ffff` | Hole; `GEM_VA` rejects it. |
| Above `0xffff_8000_0000_0000` | High range of the 48-bit linear form; the LG200 client window is 1 TiB, so the kernel publishes no high range. |

The MMU uses the low 48 bits of a 64-bit address; the upper 16 bits are ignored.
The client walk covers 40 bits. The hole is above the client window on this
generation, so a client mapping cannot be placed in it and there is no second
client range to alias.

## Page tables

A page-table entry is 8 bytes. The kernel supports 4 KiB and 16 KiB pages; the
page size is a build-time property of the kernel and is reported in the device
information.

| Bits | Field | Meaning |
|------|-------|---------|
| `[63:40]` | Reserved | Must be zero. |
| `[39:12]` | PA | Physical page base. The low 12 bits of the field are the page offset within the 4 KiB page; a 16 KiB page uses the same base alignment. |
| `[11:8]` | — | Not part of the defined format. |
| `[7:4]` | ZINF | Compression or zero-infill control. |
| `[3]` | W | Writable. |
| `[2]` | Exception | Raise an exception on access. |
| `[1]` | Huge | Huge-page entry. |
| `[0]` | Present | Entry is valid. |

The kernel maps the user flags as follows: a readable mapping sets `PRESENT`, a
writable mapping also sets `W`. The executable, partially-resident and memory-type
flags of the mapping ioctl are accepted by the interface but are not represented
in the PTE by the current kernel; a driver MUST NOT rely on them changing the
MMU behaviour.

The kernel programs a three-level walk for VMIDs 1 through 7. The walk covers a
1 TiB region with three levels; the geometry is programmed in the `DIR_CTRL`
register. VMID 0, which the kernel uses for its own control traffic, uses a
single-level GART instead.

> UNRESOLVED: whether a huge-page entry is honoured for a client mapping, and
> what the `ZINF` field controls on this generation.

## VMIDs

The device provides eight VMIDs.

| VMID | Owner | Use |
|------|-------|-----|
| 0 | Kernel | Kernel control traffic and the GART. |
| 1–3 | DRM | Graphics and blit submissions. |
| 4–7 | KCD | Compute queues. |

A VMID identifies a page-table root and a PASID. A VMID returned to the pool is
held for 20 ms before it can be reused, so a page-table walk started by the
previous owner cannot resolve against the new owner's directory.

## Segment apertures

Two segment apertures are decode tags in bits 63:48 of a 64-bit flat address.
They are outside the client walk, not blocks inside the client address space:

| Aperture | Tag and base | Size | Effect |
|----------|--------------|------|--------|
| LDS | tag `0x0001`, base `0x0001_0000_0000_0000` | 4 GiB | Redirects a flat request to on-chip workgroup shared memory. |
| Scratch | tag `0x0002`, base `0x0002_0000_0000_0000` | 4 GiB | Redirects to per-invocation private memory, expanded per work-item. |

A client mapping in the published 1 TiB window has tag 0, so a client page table
never maps either aperture. A flat request is redirected by the tag; the
aperture is a decode, not storage. The scratch bytes used by the graphics path
are an ordinary client buffer object reached through a descriptor.

The hardware takes the aperture base from bits 47:32 of the programmed value, so
the driver writes `0x0001_0000` for LDS and `0x0002_0000` for scratch. The KCD
per-process aperture begins at `0x0003_0000_0000`; that is a KCD-specific decode
base, not the DRM client VA base (1 MiB) or the user allocator's 4 GiB floor.

A shared-memory address is a byte offset within the LDS aperture. The register
form carries 32 bits and the immediate form 16 bits, so a workgroup allocation
cannot exceed 4 GiB. The kernel descriptor's granulated
shared-memory field can represent at most 2047 granules of 128 bytes, or 262,016
bytes; the actual per-workgroup LDS capacity is UNRESOLVED.

> UNRESOLVED: the per-workgroup LDS capacity and the number of workgroups that
> can be resident with a given allocation.

## Buffer objects and mappings

A buffer object (BO) is a kernel allocation with a size, a memory domain and a
handle. A user-mode driver maps a range of a BO into a GPU virtual address with
the `GEM_VA` ioctl. The mapping has a base address, an offset inside the BO and a
size; all three MUST be aligned to the mapping granularity reported by the
kernel (the larger of the CPU page size and the GPU page size).

A BO may be created in system memory, in VRAM, or in a driver-selected domain.
The domain flags and the accepted creation flags are described in
[Buffer objects](../host/buffer-objects.md). A BO that a submission reads or
writes MUST remain resident until the submission's fence signals.

## Coherency and ordering

The memory model has three rules:

1. **Explicit residency.** A submission lists the buffers it touches. The
   kernel makes them resident before the command processor starts and keeps them
   resident until the fence signals.
2. **No cache-management packets.** There is no flush or invalidate command. A
   CPU read of GPU-written memory is valid after the fence. A CPU write becomes
   visible to the GPU through the ordering of the submission that consumes it.
3. **Write-combined host mappings.** Host-written resources use write-combined
   mappings; a driver MUST NOT assume that a CPU store is visible to the GPU
   before the submission that follows it.

Within a shader, the ordering instruction drains the pending memory and
interface work named by its operand bits. It is not a host fence. The scopes
are:

| Scope | Established by |
|-------|----------------|
| Same invocation | Instruction order; the ordering instruction. |
| Same workgroup, shared memory | Workgroup barrier. |
| Same submission | Command order and the submission fence. |
| Host and device | Residency and the completion fence. |

## Atomics

Atomic instructions perform a read-modify-write on a location in shared memory,
a buffer, a flat address or an image. The operation returns the value read at
the start. Several lanes of one instruction may target the same location; the
locations are updated one lane at a time, and the result of the instruction is
the lane-order accumulation rather than an independent value per lane.

> UNRESOLVED: the lane service order for conflicting lanes, and the ordering of
> an atomic against other memory instructions in the same invocation.

## Faults and recovery

An access to an unmapped address raises a virtual-address fault. The kernel
quarantines the faulting context, records the fault, and may reset the pipe or
the device. A reset starts a new epoch: submissions admitted in an older epoch
are stale, and a completion from the old epoch MUST NOT be treated as proof that
the new device state executed the work.

The user-space interface reports the epoch and the retirement state through the
fence-status query. The flags and their meanings are in
[UAPI](../host/uapi.md).

> UNRESOLVED: which faults are recoverable per pipe, and how a compute queue's
> fault affects an unrelated graphics context.
