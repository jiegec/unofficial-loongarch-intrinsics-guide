# Compiler Contract

A generator for this device has two obligations: against the hardware, keep the
rules the encoding does not check -- which mask an instruction runs under, what
must be ordered before what, how a register tuple is aligned; against its own
later stages, leave the program in the state the next stage assumes. **Host
policy** marks one compiler's choices rather than device requirements, and
**UNRESOLVED** marks behaviour the sources do not establish, with the
consequence for a producer.

## Stage invariants

A fixed stage order runs over one intermediate program. Each stage owns a
postcondition the next stage relies on, checked separately from the structural
well-formedness every boundary gets.

| Stage hands over | Guarantee the next stage relies on |
|---|---|
| phi elimination, optimization | No phi instruction exists; the optimizer never sees one. |
| wait placement, both runs | A nonzero wait domain is a subset of the four classes. |
| uniform-load sinking | Each moved load retains a completion wait before its first reader; a shared wait remains available to the other producers. |
| allocation, SSA destruction, cleanup | Every operand is a physical register or a constant; no temporary survives. |
| value spill | A reload of a spill slot has an ordering instruction after the store that wrote it. |
| pseudo lowering | No pseudo remains and every operand is physical. |
| scheduling, both runs | The per-block instruction multiset is unchanged: same instructions, another order. |

Two properties belong to no single stage. **A read has a definition** that
dominates it, or precedes it in the same block -- the de-SSA form of the SSA
guarantee, and what a loop-carried value without a phi breaks. **The last block
ends in a terminator**, and that is checked when the program becomes bytes, not
at a stage boundary, because a middle block may fall through and the last block
may have had its fall-through branch dropped.

The two scheduling stages only reorder instructions. Uniform-load sinking is
a separate transformation because it may also insert a completion check. A
stage that succeeds without doing its job leaves well-formed IR that breaks
the next step, which is the failure the postconditions catch.

> UNRESOLVED: dominance under predication. A read under an empty lane mask
> observes nothing, so a value defined only in the region that installed that
> mask is legal without dominating the read. Whether such a program is well
> formed is not established.

## Uniform-load placement

**Host policy:** uniform-buffer row loads may move toward their first reader
within a basic block to shorten scalar-register lifetimes. A prologue that
loads many independent rows and waits once can otherwise exhaust the scalar
file even when each row is consumed separately. This is a placement problem;
it does not change the hardware register capacity or the shader ABI shared by
the graphics APIs.

A check belongs to every outstanding producer in its domain. An adjacent check
can move with a load only if no other undrained producer needs that check.
Searching backward for those producers must account for check domains: a
vector-memory check does not retire a scalar load. If a load moves past a
shared check, leave the original check with the other producers and insert a
scalar-memory check (`k = 0x1`) after the moved load, before its first reader.
Later wait placement may merge, narrow or defer these checks while preserving
the completion dependencies.

Movement stops at a reader, an address or destination redefinition, a memory
write, a workgroup barrier, a control-flow terminator, an explicit execution-mask
write, or a geometry/stream-output side effect. Uniform row loads do not use
the temporary predicates clobbered by texture or vector-buffer accesses; those
clobbers alone do not prevent movement of an independent scalar row load.
Descriptor and push-constant flat loads have different lifetime tradeoffs and
are outside this row-load sinking policy.

The first-reader position must follow the current instruction order. Cached
numeric positions become stale when earlier moves insert checks or rotate an
instruction range. Allocation failure while adding a check must preserve the
original instructions and their existing completion waits.

## Source modifiers during optimization

The two sign bits in an 11-bit vector source selector act before the
instruction: `ABS` clears bit 31 and `NEG` then flips it. They also apply to
inline constants. See [Vector Family](../isa/families/vector.md) for the
selector encoding; these are source bits, separate from `M.mod2`.

A register copy with either modifier computes a new bit pattern. Replacing a
use of that copy must compose the copy's modifiers with the use's modifiers,
in that order. An outer `ABS` replaces the inner sign operations; without an
outer `ABS`, the absolute flag survives and the two negations cancel when both
are present. This applies to every bit pattern, including signed zero,
infinity and NaN payloads.

For example, `r = -d; d = select(mask, s, r)` can become
`d = select(mask, s, -d)` when the register and execution-mask lifetimes allow
the fold. Dropping the minus changes the false arm. A select whose two data
operands name its destination is an identity only when both operands are
unmodified: `select(mask, d, -d)` still changes some lanes. Replacing a
materialized constant with an inline selector must likewise preserve the
consumer's modifiers.

These are compiler transformation requirements shared by all shader stages.
An API early return may already have become a select before instruction
selection, so validation must check the selected values as well as control
flow and register allocation.

## Condition register

SCC is one flag per wave, named by the fixed scalar selector `0x80`. A compare
with an immediate writes it and must name exactly that selector; `cb` and `czb`
read it without naming it.

- Only one SCC value can be live: a redefinition while the flag is live, or two
  live compare results, is a program error.
- SCC has no data form. To move a flag into the data domain, `s_cselect_b32`
  with 1 and 0 materialises a scalar and `v_cmp_lg_i32` against zero broadcasts
  it to a lane mask. The whole-quad expand writes the flag as a side effect.
- A uniform branch changes control flow but not the lane mask: both arms keep
  the current mask.
- A scalar subtract-with-borrow reads its borrow input as a boolean; any
  non-zero value subtracts one.
- A vector add writes its carry-out as a **lane mask**, in the scalar word
  beside the value destination, and `v_addc_u32` takes such a mask as its
  carry-in gate. A 64-bit add chain passes the low half's carry straight into
  the high half's add; materialising it as 0/1 costs three instructions and two
  constant moves.

## Lane masks

`p0..p3` are ordinary scalar registers at numbers `0x7c..0x7f`, one 32-bit word
each, one bit per lane. An instruction selects one with its 2-bit PJ2 field:
the register number minus `0x7c`, where zero is `p0`, the ordinary execution
mask. The selected mask is an **absolute lane set** -- the instruction runs for
exactly the lanes that register holds, and a narrower region is entered by
computing a new mask, not by nesting a gate.

- `p0` holds the launch's lane mask; `p2` and `p3` hold undefined data at
  entry, so the entry sequence reads `p0` only. The capture copies `p0` into a
  scalar and MUST be the first write to `p0`. A stage that needs helper lanes
  follows it with a whole-quad expand, which turns every nonzero group of four
  adjacent lanes into an all-ones quad. A restore intersects the two:
  `p0 = region_mask & saved`.
- A mask cannot be read as data: feeding `p0..p3` to a vector logical
  instruction as an ordinary source does not return the per-lane mask -- a
  1-bit select lowered as `(true & m) | (false & ~m)` answers the true arm for
  every lane. A mask that must become a value is materialised as `0xffffffff`
  or zero by `v_select_b32`, and comes back through a compare against zero.
- A compare writes a lane mask, or a scalar register holding the 32-bit ballot.
- Four registers hold at most four mask values; a value that cannot keep one is
  copied into a free register immediately before each instruction it gates.
- Some accesses rewrite the mask in effect without naming it. The set is
  narrower than any memory access: the masks stay live across flat, atomic,
  shared and sampler accesses and are redefined at **buffer** accesses, so a
  buffer access counts as a write to `p0` and a producer that needs its mask
  afterwards MUST re-install it.

### The masked destination

> UNRESOLVED: what an instruction leaves in the lanes its mask excludes. The
> mask emitter states that a gated vector write **clears** them -- a gated move
> zeroes them, it does not preserve them -- and builds a per-lane select
> wherever a value written inside a region is read outside it. The
> post-allocation liveness analysis states that a write under a narrower mask
> **keeps the previous value** on those lanes, which is why only an ungated
> write counts as redefining the register. A shared-memory access is stated to
> suppress rather than clear, so the conflict is specific to the vector
> destination.

Whichever reading holds:

- A merge MUST NOT be built from complementary gated writes. Use
  `v_select_b32` with the mask as an operand: it reads the mask per lane and
  writes every lane.
- A consumer outside a region MUST NOT assume the destination still holds its
  old value.
- A producer that needs the old and the new value across a region MUST read,
  select and write back rather than rely on the lanes it did not touch.

## Ordering

The ordering instruction drains the pending work of the classes its domain bits
name. The four bits are one ISA parameter field in the low word, split across
three subfields of the row:

| Bit | Field | Class |
|---:|---|---|
| 0 | `X0_1` | k -- kernel and scalar memory |
| 1 | `M.vm1` | vm -- vector memory |
| 2 | `X1_2[0]` | vs -- vector store |
| 3 | `X1_2[1]` | sm -- shared memory |

- The bits combine; `0xf` drains everything. An unassigned domain encodes
  `0xf`, and a nonzero one MUST be a subset of the four classes.
- It drains whole classes, not operations: a domain naming `vm` waits for every
  pending vector-memory operation, read or not.
- It is not a barrier, not a lane rendezvous and not a host fence. It MAY sit
  inside a narrowed region and orders the work issued before it, including work
  issued inside that region; placed before the region it cannot order what the
  region issues.

### Hazard classes

| Producer | Class | What the consumer needs |
|---|---|---|
| scalar flat or constant load: descriptor fetch, push constant | k | its class drained before the value is read |
| vector buffer or flat load, atomic, sampler, gather, texel load | vm | the same |
| vector store: buffer, flat, sub-word | vs | a check naming `vs` before a later load of that family |
| shared load, store, atomic | sm | the same, before the address or data register is reused |

- A load's result MUST NOT be read before a check that names its class; a
  sampler or gather is `vm`. This includes the first ALU consumer that packs
  sampled values for fragment output. A check after that packing or after an
  SIO write is too late. Restoring the execution mask does not wait for a
  texture result, and gather suffixes and array forms retain this requirement.
- A store leaves its family outstanding until a check drains it, and a store
  and a load do not share a class: a reload's own check covers the load, not
  the store before it.
- An SIO write and a geometry emit order like a store; a load MUST NOT be
  hoisted across either.
- The read of a compare's mask result MUST come after a check too: a compare
  and the select that reads it, emitted back to back, read the previous mask.
- A check MAY move down over lane-ALU and scalar-ALU rows, which touch no
  memory and have no side effect, but not past a reader of a pending result, a
  redefinition of a pending value, or a rewrite of a register a pending
  operation reads. It is legal only while it still dominates every consumer. A
  check whose pending set is already empty MAY be dropped; a block-entry check
  is kept.

## Descriptor staging

An SIO write reads its channel descriptor from the VGPR whose **number** equals
the channel slot. The hardware reads that register, not an operand, so a
producer MUST place the descriptor in `v<slot>` before the write, and a pass
that removes dead moves MUST treat that register as read or the staging move
disappears. The slot is a 12-bit immediate in the immediate form and the `VK8`
field in the register form.

| Operand | Width | Base |
|---|---|---|
| coordinate tuple | 3-5 consecutive VGPRs; 8 for shadow gradient with offset, 4-10 for the gradient form | consecutiveness, and a multiple of four for the array form |
| image descriptor | 8 consecutive SGPRs | a multiple of eight |
| sampler descriptor | 4 consecutive SGPRs | a multiple of four |
| result | 4 consecutive VGPRs | a multiple of four |

The descriptor fields are 8-bit and 4-bit register numbers, so an eight-wide
tuple cannot start above 248 and a four-wide one not above 252. A destination
component mask is 4 bits and MUST be non-zero on the forms that carry one.

### Tuples

| Width | Base alignment | Examples |
|---|---|---|
| 2 | even register | 64-bit value, address pair |
| 4 | multiple of four | sampler descriptor, vec4 result, vec4 data |
| 8 | multiple of eight | image descriptor |

- A 64-bit vector source is either an aligned VGPR pair, whose base occupies
  the ordinary selector, or a single inline constant replicated across both
  halves.
- A descriptor operand names the base of an implicit register window. Buffer
  and sampler descriptors read four consecutive SGPRs; an image descriptor
  reads eight. For base `b` and width `W`, every register in `[b, b + W)` is a
  source dependency, including image words `b + 4` through `b + 7`.
- Register allocation and scheduling MUST preserve both directions of that
  dependency. A texture access cannot move above an instruction that fills
  any descriptor word. A later load that reuses those SGPRs cannot move above
  the texture access either: it would replace the descriptor before use.
  Test these edges separately from an independent load that may still move.
  The width belongs to each source operand; a four-word sampler window does
  not bound the adjacent image operand's eight-word window.

> UNRESOLVED: whether a misaligned tuple is representable at all, and what a
> misaligned base does. Usage shows aligned bases only, and an allocator that
> never produces a misaligned one proves nothing about the encoding.

## Host policy

- The low twelve SGPRs are reserved for the launch window: `s0-s3` host the
  lane-mask allocation window and `s4-s11` the graphics ABI inputs. The
  graphics constant root is `s4:s5`; compute has none and reads its kernarg
  pointer from `s[6:7]`.
- A mask value is filled at the nearest common dominator of the instructions it
  gates, so one register can serve two arms of a branch that never run
  together.

## Open questions

- Whether the lanes a mask excludes are cleared or preserved.
- Whether `p1` has a defined value at entry; the sources name only `p2` and
  `p3` as undefined.
- Which wait class an SIO write and a geometry emit belong to; their class is
  not established, so a check covering them keeps the full domain.
- Whether a misaligned tuple base is rejected, aliased, or addresses another
  register pair.
