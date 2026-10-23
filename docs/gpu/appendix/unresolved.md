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

The instruction rows carry their own: **525 of the 768** machine rows state at
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
| ~~The semantics of `PJ2`~~ ([Encoding](../isa/encoding.md), [Operands](../isa/operands.md)) | **Closed 2026-10-21/22.** `PJ2=k` selects `pK` as the instruction's absolute lane mask; the mask survives the instruction (ALU, flat, shared memory and the fragment stage), compares can write a mask register while selecting it, `cb`/`czb` read one, `s_wqm_b32` takes and returns one, the mask file is not a window of the scalar file, and a gated write suppresses the write instead of clearing the lane — so two gated copies with complementary masks compose into a per-lane merge.  An emitter writes the selector it means; a decoder should print it as the mask operand. |
| The shared-memory address unit and lane mapping ([Operands](../isa/operands.md), [Atomic family](../isa/family-atomic.md), [Memory family](../isa/family-memory.md)) | Workgroup addressing cannot be generated, and the matrix plane layout cannot be read, without it. |
| The operand form of 8- and 16-bit buffer loads and stores ([Operands](../isa/operands.md)) | Narrow vertex attributes and narrow storage access have no verified form. |
| Where the atomic counter bound is read from ([Atomic family](../isa/family-atomic.md)) | **Partly wrong before; corrected 2026-10-13.**  A field DOES carry it: the row's own semantics say the source operand IS the bound (`new = (old < b) ? old + 1 : 0`), and the seed `atomic_sm_inc_u32 p0, v0, v0, v0` passes one.  What is true is that no PRODUCER path exercises it: compiling OpenCL `atomic_inc`/`atomic_dec` with the closed backend lowers them to **add/sub with 1** - `atomic.v.flat.add.i32`, `atomic.sm.add.u32`, `atomic.v.flat.sub.i32` - and the `*_inc_*` / `*_dec_*` instructions are never emitted at all.  So the bound is encodable but unreached, and the open half is the hardware's behaviour AT it. |
| The dispatch dimension roles ([Compute](../host/compute.md)) | A compute launch cannot be dimensioned correctly; a driver must not assume X, Y, Z ordering. |
| The environment index that selects each identifier ([Compute](../host/compute.md), [Scalar family](../isa/family-scalar.md)) | A compute shader cannot compute its global or group-relative position reliably. |
| The surface record's alignment code and base-layer meaning ([State Object](../graphics/state-object.md)) | Mip and layered attachment setup is guesswork outside the two established alignment cases. |
| The modifier semantics `mod2`, `clmp1`, `bl1`, `bl2` ([Operands](../isa/operands.md)) | Two halves are now distinct: `mod2` and `clmp1` are emitted at ZERO everywhere the producer goes (no intrinsic exposes them at all), and `bl1` appears only on the compiler's private spills while `bl2` is unused - so the ROUNDING direction is selected by the opcode (`v_ceil.f32`, `v_floor.f32`, ...), not by a modifier.  What a non-zero `mod2` would do is still unsupported. |
| The interpolation phase arithmetic ([Interpolation family](../isa/family-interpolation.md)) | Only the payload move is usable; a varying must come from the stage interface instead. |
| How many waves a unit can hold ([Generation Profile](../chips/lg200.md)) | Occupancy budgeting cannot be done from the ISA.  The wave size of 32 lanes IS settled as an architectural maximum: the lane mask is one 32-bit word (measured - an all-true run gives 0xffffffff), so a 33rd lane would have no representation. |

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

One gap is narrower than it looks: the mask campaign could not ask whether the
**texture/sampling** forms and the **SIO** transfers gate by `PJ2`, because the
structured carrier refuses the texture family (a single-instruction run has no
bound image or sampler tuple) and no SIO transfer was executed under a mask.  Both
of those layouts do carry the field, so the open part is the gating behaviour, not
the encoding.

## How to read a statement

A sentence in this guide states what has been established for the exact variant
under discussion.  A neighbouring operand form, width, modifier or tuple of the
same mnemonic is a different question and does not inherit the answer.  Where a
row's behaviour diverges from a comparable architecture, the divergence is stated
in the row's own description as a difference rather than as a general rule.
