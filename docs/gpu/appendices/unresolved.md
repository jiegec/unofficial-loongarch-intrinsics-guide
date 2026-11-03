# UNRESOLVED Register

A chapter marks a property it cannot establish as `UNRESOLVED` where that property appears. This appendix collects those properties by area and states the decision each one blocks. It is a work list for a driver, compiler or shader author, not a second description of the device: the mechanism stays in the chapter that owns it.

An entry is a question, never a guess. A host that must act picks one of the possible behaviours and records it as **Host policy**, naming the outcome it assumes; it does not present that choice as a device property. The second column says whether the entry is open because no source states the behaviour (*not stated*) or because two sources give different behaviours (*conflicting*). An entry is deleted when the behaviour is established, and the register keeps no history of closed items. A property that one host has merely not implemented is a gap in that host, not in the manual, and is not listed here.

## Execution and scheduling

Wave composition, the mask registers and the issue order are in [Execution and Scheduling](../architecture/execution-model.md); the instruction forms that read a mask are in [Execution](../isa/execution.md).

| Property | Why open | Consequence |
|----------|----------|-------------|
| Maximum lanes per wave. | not stated | 32 is the width the ABI and the supported kernels use, not a proven ceiling. A compiler MUST size a mask register at 32 bits and a host MUST report a subgroup size of 32. |
| How many waves a workgroup that contains a barrier may hold. | not stated | The device reports four SIMDs per compute unit at ten waves each, so forty waves fit one unit; whether a barrier-bearing workgroup may occupy all of them is not established. A workgroup ceiling is host policy, and a host MUST NOT launch above the ceiling it advertises. |
| Issue width and per-instruction latency. | not stated | No latency is encoded in an instruction and none is published, so a scheduler cannot be tuned to a documented model. Ordering MUST come from the explicit ordering instructions, never from an assumed latency. |
| Whether an odd-base vector tuple is legal. | not stated | An operand pair is named by its base register. A compiler MUST align a tuple to its width, and a decoder MUST report an odd base in a tuple field rather than accept it. |
| What an unselected lane holds after a gated write. | conflicting | Both "the lane keeps its previous value" and "the write clears the lane" are stated, while a gated load suppresses its update instead of clearing. A compiler MUST NOT build a merge from complementary gated writes; it MUST use a per-lane select with the mask as an operand. |
| Which instruction classes clobber the mask registers without naming one. | conflicting | A buffer access redefines all four. One rule counts image and flat accesses with it, another keeps the masks live across them. A shader that needs a mask across any memory access MUST re-establish it. |
| The value `p1` holds at wave entry. | not stated | `p0` holds the launch mask and `p2`/`p3` are undefined. A shader MUST capture `p0` before it writes `p0`, and MUST NOT read `p1`..`p3` before writing them. |
| The value a writer leaves in `SCC`. | conflicting | One family leaves `0xffff` for true and the immediate family leaves 1. A consumer MUST test zero against non-zero and MUST NOT compare `SCC` with a particular non-zero value. |
| Whether a compare that writes the mask it selects reads the old mask first. | not stated | A compare may name a mask register as its destination while its own selector names that same register. A compiler MUST write a compare result to a register other than the one the same instruction selects. |

## Memory and addressing

Address formation, the decode tags and the fault model are in [Memory System](../architecture/memory-system.md); buffer objects are in [Buffer Objects](../host/buffer-objects.md).

| Property | Why open | Consequence |
|----------|----------|-------------|
| Whether the MMU honours a huge-page entry for a client mapping. | not stated | Page tables are built from single-page entries and no source describes a larger one. A driver MUST map a client range in single pages. |
| The hardware's per-workgroup shared-memory ceiling. | not stated | The launch descriptor names the window in 128-byte granules with a bias, and the device reports 128 KiB per compute unit; how that is split per workgroup is not established. The advertised window is host policy, and a shader MUST size its shared allocation to it. |
| Out-of-range buffer and image access. | not stated | Whether the access is dropped, clamped to the last element or faults is not established. A host MUST keep every access inside its descriptor, and robust access MUST NOT be promised. |
| The order of two lanes of one instruction that write one address. | not stated | A read-modify-write from two lanes of the same instruction has no documented order. A shader MUST reduce across lanes before the write, or use one atomic operation per result. |
| Which faults are recoverable per pipe. | not stated | Recovery is at context granularity. Whether an unrelated context survives a fault raised on another pipe is not established. |

## Host interface and submission

Ring framing, cursor writeback and doorbells are in [Rings](../host/rings.md) and [MMIO](../host/mmio.md); submission lifetime is in [Submissions](../host/submissions.md).

| Property | Why open | Consequence |
|----------|----------|-------------|
| Whether a command processor requires the start alignments a host reports. | not stated | Admission guarantees 4-byte alignment only. A producer SHOULD align a buffer packet to the larger boundary it reports, and MUST NOT depend on a rejection when it does not. |
| Whether a packet may straddle the end of a ring. | not stated | A producer MUST keep every packet contiguous and pad the tail. |
| Whether a fence writeback retires when the pipe issues it or when the write drains. | not stated | A waiter MUST treat the fence as ordering only once the word is written, and MUST NOT infer completion from the packet's position in the ring. |
| The read-cursor update granularity of each pipe. | not stated | A producer MUST NOT reuse ring space before the cursor has passed it, whatever the reported granularity. |
| Whether a user queue can require the context-save fields to be acted on. | not stated | Context save is not exposed to a user queue, and a queue MUST NOT assume its registers survive a preemption or a reset. |
| The meaning of the `ZIP`, `EXC`, `CWSR` and `VRAM` immediate operations. | not stated | A user command stream MUST NOT issue them. Their effect on the pipe that issues them is not empty, and it is not described here. |
| Which state a per-pipe reset preserves. | not stated | Recovery is at context granularity. A driver MUST rebuild the state it needs after a reset rather than rely on preservation. |
| The unit and wrap behaviour of the time counter. | not stated | A calibration against a host clock is required, and the difference between two readings that straddle a wrap is not defined. |

## Graphics pipeline

Primitive assembly and the stage order are in [Pipeline](../graphics/pipeline.md); raster state is in [Rasterization](../graphics/rasterization.md) and [Depth and Stencil](../graphics/depth-stencil.md).

| Property | Why open | Consequence |
|----------|----------|-------------|
| Per-attribute instance divisor. | not stated | The vertex-input record carries no divisor field, so no attribute advances by a per-instance step. A host MUST replicate the attribute per instance or compute the step in the shader. |
| Whether a scissor rectangle includes its maximum edge. | not stated | A one-pixel difference at the edge is possible. A test MUST NOT assert the boundary, and a producer SHOULD keep the rectangle half-open. |
| Subpixel precision and the shared-edge fill rule. | not stated | Two triangles that share an edge are not guaranteed to cover each pixel exactly once. A shader that writes coverage MUST NOT rely on the rule. |
| Sample positions under multisampling. | not stated | The pattern cannot be reproduced exactly, so a shader MUST NOT depend on a specific sample location. |
| The rule that an early depth test applies. | not stated | The Early Z bit exists, but which comparison runs and against which written value is not stated. A shader whose fragment writes a side effect MUST disable early depth. |
| Depth or stencil with only one surface bound. | not stated | The legal attachment combinations are not closed. A producer MUST bind both surfaces or neither. |
| Whether a floating-point depth is clamped, and at which point. | not stated | The clamp-near and clamp-far bits exist, but their effect on a floating-point depth is not stated. A shader MUST NOT rely on the clamp. |
| Stencil increment and decrement wrap boundaries. | not stated | Where a saturated increment stops, and whether a wrap applies per face, is not stated. A producer SHOULD keep the reference inside the range it can observe. |
| Whether a blit may read and write one surface, and how it converts tilings. | not stated | A host MUST NOT alias a blit's source and destination, and a blit between two different tilings is not specified, so a host MUST convert through a linear surface or refuse. |

## Textures and formats

Image and sampler records are in [Textures](../graphics/texture.md); surface layout and pixel formats are in [Formats](../graphics/formats.md).

| Property | Why open | Consequence |
|----------|----------|-------------|
| The meaning of the performance-mode, performance-MIP and performance-Z fields. | not stated | No value is established to have an effect. A host MUST NOT tune them and MUST write the value the described path writes. |
| The alignment code beyond the values used for a cube surface and the constant 6. | not stated | Mip and layered setup is conservative. A host MUST use a value whose behaviour is described. |
| Whether anisotropic filtering has any effect. | not stated | The capability is not advertised, so a sampler MUST NOT request a maximum anisotropy above 1. |
| The per-quad granularity of the bias and depth-reference slots. | not stated | Only the offset slot is documented as per-quad. A shader MUST NOT rely on a per-quad bias or depth reference. |
| The sample layout of a 2x depth surface. | not stated | Only the 4x layout is described, so a 2x depth surface MUST NOT be sampled or resolved. |

## Shader ABI

The shader-info record and its roots are in [Shader ABI](../shaders/shader-abi.md); the register namespaces are in [Registers](../shaders/registers.md); interpolation is in [Interpolation](../shaders/interpolation.md).

| Property | Why open | Consequence |
|----------|----------|-------------|
| Whether the VGPR initial count is a count of preloaded registers or an index. | not stated | The field is host policy today. A shader MUST NOT read a vector register it did not write, except for the identifiers the ABI names. |
| The contents of `s10:s11` at a vertex launch. | not stated | The allocation window withholds them and no documented root writes them. A shader MUST NOT read `s10:s11` as an input. |
| Whether a compute launch's reported scalar count must cover `s11..s13`. | not stated | The hardware writes the group ids there whatever the count says, so a launch applies a floor. A producer MUST report at least that floor. |
| Whether the graphics `LDS Size` field is honoured. | not stated | A graphics record carries zero in the field. A graphics shader MUST NOT declare shared memory. |
| Whether replacing code at one address requires an instruction-cache invalidate. | not stated | A replacement at the same address is not documented. A host SHOULD place new code at a fresh address and flush the range it replaces. |
| The encoding and effect of the `mod2`, `clmp1`, `bl1` and `bl2` fields. | not stated | The fields are host policy. A producer MUST write only the combination the described path writes. |
| The arithmetic of the second interpolation phase. | not stated | A varying MUST be taken from the stage interface; a shader MUST NOT reconstruct the phase itself. |
| The hardware-register numbering that `getreg` reads. | not stated | Compute identifiers come from the kernarg block, so a shader MUST NOT read a hardware register through `getreg`. |

## Instruction encoding and semantics

The mask of a row and the format rules are in [Encoding](../isa/encoding.md); operand fields are in [Operands](../isa/operands.md).

| Property | Why open | Consequence |
|----------|----------|-------------|
| How the machine selects a format when a word matches rows in more than one format. | not stated | A disassembler that is not told the format MUST report the ambiguity, and a producer MUST NOT rely on one resolution. |
| What the machine reads from an ignored bit, and whether a reserved bit must be zero. | not stated | A decoder MUST NOT match on either, and an encoder SHOULD write the base value in both. |
| The unit of the `jirl` and `trap` displacement, and the scaling of the `jpush` and `jpop` position. | not stated | A producer MUST NOT reuse the branch rule, and MUST compute the target in the unit the row states. |
| Whether an opcode field encodes anything below the row level. | not stated | The 4-bit `sio-id` and `sio-imm` fields and the 2-bit `prefix` field are used as opcode bits only, so a decoder MUST treat them as part of the mnemonic. |
| Whether a prefix reaches one instruction, persists, or composes with the next. | not stated | A producer MUST NOT emit a prefix that is not consumed by the instruction that follows it. |
| Whether a source with both the negate and absolute modifiers negates the absolute value or takes the absolute of the negation. | conflicting | A producer SHOULD build the result in two instructions instead of relying on the combination. |
| Whether an SIO transfer is gated by its `PJ2` field. | not stated | The SIO formats carry the field, while the gating rule names only the vector, flat, buffer, image and shared-memory families. A shader MUST NOT rely on an SIO write being masked. |
| Whether `PJ2` gates `jpush` and `jpop`. | not stated | Both carry the field. A producer MUST NOT rely on a gated jump-stack operation. |
