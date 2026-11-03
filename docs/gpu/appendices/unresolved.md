# UNRESOLVED Register

This register lists the properties that the manual does not establish. Each item
states the question and its consequence. An item is removed when the property is
established; the manual does not keep a history of closed items.

## Execution and registers

| Item | Consequence |
|------|-------------|
| Resident waves per vector unit. | Occupancy cannot be budgeted. |
| Issue width and per-instruction latency. | Scheduling cannot be tuned to a documented model. |
| Number of vector units and their lane counts. | Throughput cannot be derived from the ISA. |
| Whether an odd-base vector tuple is legal. | A compiler conservatively aligns pairs. |
| Whether the lane-mask window aliases the descriptor scratch in `s0..s3`. | A compiler reserves the whole window. |
| Whether a compare that writes the mask it selects reads the old mask in every format. | The result is documented for the exercised forms only. |
| Whether an SIO transfer is gated by the mask selector. | Masked SIO writes are untested. |

## Memory

| Item | Consequence |
|------|-------------|
| Huge-page entry behaviour for a client mapping. | A huge-page mapping cannot be used with confidence. |
| The `ZINF` field's effect. | Compression and zero-infill cannot be configured. |
| Per-workgroup LDS capacity. | A workgroup allocation cannot be sized against a documented limit. |
| The shared-memory offset unit and lane mapping of the swizzle forms. | Swizzled shared accesses cannot be generated. |
| The matrix partial-fill rule. | Only the documented wide-enough launches are safe. |
| Out-of-range buffer or image access behaviour. | A host must keep accesses inside the descriptor. |
| Fault recovery per pipe. | A fault's effect on an unrelated context is unknown. |

## Host and commands

| Item | Consequence |
|------|-------------|
| Exact accepted creation flags for every BO domain. | A caller must treat a rejected flag as an error. |
| `GEM_USERPTR` behaviour when the user mapping is unmapped in flight. | User-pointer memory is not a stable substitute for an import. |
| Ordering between a timeline signal and a fence writeback. | A submission's last signal order is not fully documented. |
| Whether a submission may contain more than one fence chunk. | A driver emits one. |
| Doorbell acknowledgement and queue read-index update. | A queue's flow control is conservative. |
| CWSR contract on LG200. | Context save is not exposed. |
| The meaning of the `ZIP`, `EXC`, `CWSR` and `VRAM` immediate operations. | The kernel's use is opaque to a user driver. |
| The exact state preserved by a per-pipe reset. | Recovery is handled at context granularity. |
| The time counter's unit and wrap behaviour. | Calibration against a host clock is required. |

## Graphics pipeline

| Item | Consequence |
|------|-------------|
| Per-attribute instance divisor. | Instanced attributes must be replicated or handled in the shader. |
| Out-of-range vertex fetch behaviour. | A fetch must stay inside the descriptor. |
| Scissor edge inclusion. | A one-pixel difference at the rectangle edge is possible. |
| Subpixel precision and the shared-edge fill rule. | Exact rasterization of shared edges is not specified. |
| Sample positions for multisampling. | The sample pattern cannot be reproduced exactly. |
| Early-depth and hi-Z acceptance rules. | Early depth cannot be relied on for correctness. |
| Guardband interaction with clip and cull planes. | Clip behaviour near the guardband is conservative. |
| Adjacency primitive behaviour. | Adjacency topologies are not a reliable path. |
| Depth or stencil with only one surface bound. | The legal attachment combinations are not closed. |
| Near/far clamp on floating-point depth. | Floating-point depth clamp is not specified. |
| Stencil wrap and clamp boundaries. | The exact increment and decrement limits are not specified. |
| Legal dual-source blend factors. | Dual-source blending is conservative. |
| Logic operation with several render targets. | Multi-target logic operations are not specified. |
| Alpha-to-coverage sample counts other than 4x. | Alpha-to-coverage requires 4x. |
| 2x multisample support and layout. | Only 4x is exposed. |
| Streamout stage hardware contract. | The stage record is not used by the current path. |
| Whether a blit can read and write the same surface. | A host must not alias source and destination. |
| Tiling conversion rules for a blit. | A blit between different tilings is not specified. |

## Textures and formats

| Item | Consequence |
|------|-------------|
| The performance-mode field. | The value 4 is written without a meaning. |
| The 12-bit colour swizzle subfields. | Only per-format constants are known. |
| The alignment code beyond cube and the constant 6. | Mip and layered setup is conservative. |
| Whether anisotropic filtering has any effect. | The capability is not advertised. |
| The per-quad granularity of the bias and depth-reference slots. | Only the offset slot is documented as per-quad. |
| The border-table addressing unit. | The table pointer is written in 8-byte units. |
| Surface types 2 and 6. | Mip-mapped attachment selection is conservative. |
| Tiling values other than 0 and 2. | Only linear and tile2 are used. |
| The 2x depth sample layout. | Only 4x depth is documented. |

## Shader ABI and ISA

| Item | Consequence |
|------|-------------|
| The VGPR initial count's meaning. | The field is programmed by convention. |
| Hydration of `s10:s11`. | The register is not used as a documented input. |
| Whether the graphics LDS size field is honoured. | The graphics path writes zero. |
| Whether the instruction cache needs an invalidate after a code replacement. | A replacement at the same address is not documented. |
| The encodings and effects of `M.mod2` and `M.clmp1`. | The modifiers are emitted at zero. |
| The meaning of `bl1` and `bl2`. | The bits are emitted by convention. |
| The exact effect of the mask rows. | They are used only where their names suffice. |
| The second interpolation phase's arithmetic. | A varying must come from the stage interface. |
| The opcode fields of formats 9 and 10 beyond the mnemonic. | The fields are treated as opcode bits. |
| The jump-stack predicate's meaning. | The instruction is not used for a return address. |
| The prefix operations' effects. | Prefixes are used only fused to their operation. |
| The shared-memory copy granularity and address form. | The rows are described but not used. |
| The `getreg` hardware-register numbering. | Compute identifiers come from the kernarg block. |
