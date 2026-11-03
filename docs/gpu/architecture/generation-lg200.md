# LG200 Generation Profile

This chapter collects the values, limits and feature support of the LG200
generation. Mechanism chapters describe behaviour; this page holds the numbers.
A value marked **implementation** is a choice of the current driver and is not a
hardware limit.

## Identity

| Property | Value |
|----------|-------|
| Generation | LG200 |
| Shader instruction words | Two 32-bit words per instruction |
| Instruction variants | 768 machine rows |
| Encoding formats | 12 |
| Instruction families | 7 |
| Graphics stages | Vertex, domain (vertex role), geometry, fragment, streamout |
| Tessellation | Not supported |

## Pipes

| Pipe | ICMD MOP | Ring cursors (write/read) | VMIDs | Role | Completion |
|------|---------:|---------------------------|-------|------|------------|
| GPIPE | `0x01` | `0x18` / `0x1c` | 1–3 | Graphics ring and indirect buffers | IRQ source `0x01` |
| BPIPE | `0x02` | `0x30` / `0x34` | — | Inner graphics commands and blits | IRQ source `0x02` |
| CPIPE | `0x03` | `0x38` / `0x3c` | 4–7 | Compute queues and user queues | IRQ source `0x22` |
| XDMA | `0x04` | `0x20` / `0x24` | — | Linear copy and fill | IRQ source `0x23` |
| DPIPE | `0x0c` | `0x48` / `0x4c` | — | Not used on LG200 | Not assigned |
| EPIPE | `0x0d` | `0x50` / `0x54` | — | Not used on LG200 | Not assigned |

The register offsets of the command interface are in [MMIO](../host/mmio.md).

## Execution resources

| Resource | Value |
|----------|-------|
| Wave width | 32 lanes |
| Scalar registers | 102 (`s0..s101`) |
| Vector registers | 256 per lane (`v0..v255`) |
| Lane-mask registers | 4 (`p0..p3`) |
| Condition register | 1 bit per wave (`SCC`) |
| Maximum instruction stream | 64 KiB code-size field; the compiler limits a binary to 8191 instructions |
| Instruction alignment | 8 bytes |

The number of vector units, their lane counts and the resident-wave capacity are
UNRESOLVED; see [Execution model](execution-model.md).

## Memory

| Property | Value |
|----------|-------|
| Client virtual address width | 40 bits (1 TiB); the linear form is 48 bits |
| Page-table entry | 8 bytes |
| Page size | 4 KiB or 16 KiB, a kernel build property |
| Walk | Three levels for VMIDs 1–7; one-level GART for VMID 0 |
| VMIDs | 8: kernel 0, DRM 1–3, KCD 4–7 |
| Excluded VA head (floor) | 1 MiB |
| VA hole | `0x0000_8000_0000_0000` – `0xffff_7fff_ffff_ffff` |
| LDS aperture | `0x0001_0000_0000_0000`, 4 GiB |
| Scratch aperture | `0x0002_0000_0000_0000`, 4 GiB |
| Mapping granularity | `max(PAGE_SIZE, GPU_PAGE)` |
| VMID reuse delay | 20 ms |

## Graphics capabilities

| Capability | Value |
|------------|-------|
| Colour attachments | 8 |
| Depth attachments | 1 |
| Viewport records | 16; the current driver uses viewport 0 |
| Vertex attributes | 16 |
| Vertex buffers | 16 |
| Uniform buffer bindings per stage | 12 |
| Storage buffer bindings | 16 |
| Push constants | 256 bytes |
| Clip distances | 8 |
| Fragment attribute map entries | 32 |
| Interpolation coefficients | 6-bit count, 0–63 |
| Sampler descriptors per shader | 16 |
| Mip levels | 16 |
| Array layers | 256 |
| Multisampling | 2x and 4x are encodable; the current driver exposes 4x only |
| Anisotropic filtering | Not advertised by the current driver |
| Tessellation | Not supported |
| Compressed textures | S3TC is decompressed by the hardware into the RGBA8 slots |

## Instruction set

| Family | Rows | Machine formats |
|--------|-----:|-----------------|
| Vector ALU | 374 | `vop`, `vop-imm`, `shared-memory` |
| Scalar ALU | 119 | `sop`, `sop-imm-control` |
| Memory and image | 138 | `smem`, `tex-buffer`, `flat`, `shared-memory` |
| Atomic | 97 | `tex-buffer`, `flat`, `shared-memory` |
| Control and ordering | 20 | `sop-imm-control`, `jump-stack`, `shared-memory`, `prefix` |
| SIO and export | 17 | `sio-id`, `sio-imm` |
| Interpolation | 3 | `vop` |

The families are a documentation grouping, not an encoding property. A row's
machine format is independent of its family.

## Host limits

| Property | Value |
|----------|-------|
| Indirect-buffer block | 528 dwords; 517 used in the full-state form |
| Maximum IB length | `0x00ff_ffff` dwords |
| Submission chunks | 1–4096 |
| Maximum chunk size | 4 MiB |
| Fence buffer | Exactly one page |
| Resident buffers per submission | Implementation limit; see [Submissions](../host/submissions.md) |

## Open questions

| Item | Consequence |
|------|-------------|
| Number of vector units and lanes | Occupancy and throughput cannot be derived from the ISA. |
| Resident waves per unit | Occupancy budgeting is not possible. |
| Per-workgroup LDS capacity | A workgroup allocation cannot be sized against a documented limit. |
| Issue width and latency | Scheduling and instruction selection cannot be tuned to a documented model. |
| Full large-page behaviour | A huge-page mapping cannot be used with confidence. |
| DPIPE and EPIPE contracts | These routes have no user-visible payload contract on LG200. |
