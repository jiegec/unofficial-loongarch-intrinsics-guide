# UNRESOLVED Register

`UNRESOLVED` is this guide's marker for a property that has not been established.
It is written **in place**, next to the claim it qualifies, so that a reader never
has to hold a second table in their head to know how much of a sentence to
believe.  This page is an index of those places, not a second copy of them: it
says where to look, and which of the open items actually block work.

## Where the open items are

| Chapter | Open items |
|---------|-----------:|
| [Execution Model](../concepts/execution-model.md) | 7 |
| [Host Interface](../host/interface.md) | 4 |
| [Stream Command Word](../host/stream.md) | — |
| [Graphics Submission](../host/graphics-submission.md) | 5 |
| [GPIPE](../host/pipes/gpipe.md) / [BPIPE](../host/pipes/bpipe.md) / [XDMA](../host/pipes/xdma.md) / [CPIPE](../host/pipes/cpipe.md) | — |
| [Compute](../host/compute.md) | 6 |
| [State Object](../graphics/state-object.md) | 9 |
| [Draw and Rasterization](../graphics/draw-raster.md) | 6 |
| [Depth, Stencil and Blend](../graphics/depth-blend.md) | 6 |
| [Shader ABI](../shader/abi.md) | 6 |
| [Stage Interfaces](../shader/stages.md) | — |
| [Registers and Allocation](../shader/registers.md) | 5 |
| [Descriptors](../shader/descriptors.md) | 8 |
| [SIO](../shader/sio.md) | 6 |
| [Encoding](../isa/encoding.md) | 3 |
| [Operands and Registers](../isa/operands.md) | 9 |
| [Vector ALU (family)](../isa/family-vector.md) | 8 |
| [Scalar ALU (family)](../isa/family-scalar.md) | 5 |
| [Memory and Image (family)](../isa/family-memory.md) | (stated in the section on shared memory) |
| [Atomic (family)](../isa/family-atomic.md) | 10 |
| [Control Flow (family)](../isa/family-control.md) | 7 |
| [SIO and Export (family)](../isa/family-sio.md) | 6 |
| [Interpolation (family)](../isa/family-interpolation.md) | 7 |

The instruction rows carry their own: **582 of the 768** machine rows state at
least one property marked `UNRESOLVED` in the row's own description, because a row's case
vectors pin one exact variant and leave its neighbours open.  Those are recorded
on the row pages, not here.

**Progress is tracked in [UNRESOLVED Status](unresolved-status.md)**, which records
each class that has since been closed, the evidence that closed it (hardware
measurement, a closed-producer census, or the producer's own contract), and the
editing rules every writeback follows.  The counts above are the live ones; the
status page is the history.

## What is open that blocks work

A driver or a compiler can be written without most of the items above.  These are
the ones that force a decision, ordered by how much they constrain the code:

| Open item | Consequence |
|-----------|-------------|
| The semantics of `PJ2` ([Encoding](../isa/encoding.md), [Operands](../isa/operands.md)) | Every layout carries the field and no established producer uses it; an emitter must write the observed value and a decoder must not print it as an operand. |
| The shared-memory address unit and lane mapping ([Operands](../isa/operands.md), [Atomic family](../isa/family-atomic.md), [Memory family](../isa/family-memory.md)) | Workgroup addressing cannot be generated, and the matrix plane layout cannot be read, without it. |
| The operand form of 8- and 16-bit buffer loads and stores ([Operands](../isa/operands.md)) | Narrow vertex attributes and narrow storage access have no verified form. |
| Where the atomic counter bound is read from ([Atomic family](../isa/family-atomic.md)) | The increment and decrement forms cannot be emitted at all: no encoded field carries the bound. |
| The dispatch dimension roles ([Compute](../host/compute.md)) | A compute launch cannot be dimensioned correctly; a driver must not assume X, Y, Z ordering. |
| The environment index that selects each identifier ([Compute](../host/compute.md), [Scalar family](../isa/family-scalar.md)) | A compute shader cannot compute its global or group-relative position reliably. |
| The surface record's alignment code and base-layer meaning ([State Object](../graphics/state-object.md)) | Mip and layered attachment setup is guesswork outside the two established alignment cases. |
| The modifier semantics `mod2`, `clmp1`, `bl1`, `bl2` ([Operands](../isa/operands.md)) | The fields are emitted at fixed values; changing one is unsupported. |
| The interpolation phase arithmetic ([Interpolation family](../isa/family-interpolation.md)) | Only the payload move is usable; a varying must come from the stage interface instead. |
| Whether the wave size of 32 is an architectural maximum ([Generation Profile](../chips/lg200.md)) | Occupancy and register-budgeting decisions rest on a measured size, not a limit. |

## Coverage gaps, which are not the same thing

Some rows are `UNRESOLVED` because the behaviour is unknown, and some are open
because the tooling cannot stage the launch state a row needs.  The second kind is
a limit of the verification apparatus rather than of the architecture, and it is
described in [Verification Tooling](verification.md): rows needing
flat memory, formatted buffer loads, a matrix read, or a texture fetch or sample
execute in **observed** mode, so they record what happened rather than matching an
independent model.

Two further gaps are structural rather than per-row:

* the fragment-stage carrier runs one stage shape, so anything needing a second
  command stream, another pipe or another stage is outside its coverage;
* the compute path is the only way to stage a workgroup allocation, which is why
  the matrix rows were measured through it.

## How to read a statement

A sentence in this guide states what has been established for the exact variant
under discussion.  A neighbouring operand form, width, modifier or tuple of the
same mnemonic is a different question and does not inherit the answer.  Where a
row's behaviour diverges from a comparable architecture, the divergence is stated
in the row's own description as a difference rather than as a general rule.
