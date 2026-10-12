# Operands and Execution Registers

This chapter defines the operand classes shared by the machine formats, the
register files behind them, and the mechanism that decides which lanes an
instruction acts on.

## Operand classes

| Class | Fields | Meaning |
|-------|--------|---------|
| Vector | `VJ8`, `VK8`, `VA8`, `VQ8`, `VD8` | a VGPR index, or a row-defined zero |
| General source | `GJ8`, `GK8`, `GJ11`, `GK11`, `GA11` | a selector from the table below |
| Scalar / descriptor | `RJ8`, `RK8`, `RA8`, `RD8` and their tuples | scalar register, descriptor base, result or VCC position, per the row |
| Immediate | `IJ32`, `IK32`, `IA16`, `IJ16`, `IK16`, `UO8`, `UO12`, `UO20` | literal or offset |
| Modifier | `M.*` | an operation or addressing control |
| Predicate | `PJ2` | present in most layouts; semantics UNRESOLVED |

A tuple suffix names consecutive registers: `RJ8x4` is four consecutive scalar
registers starting at the encoded index, `VJ8x8` is eight consecutive vector
registers.  Tuples are aligned — a four-register tuple starts at a multiple of
four, an eight-register tuple at a multiple of eight — and the encoded value is
the base of the group.

## The selector matrix

An 8-bit general-source field carries a base selector:

| Selector | Meaning |
|----------|---------|
| `0x00..0x65` | ordinary scalar registers `s0..s101` |
| `0x66` | constant zero |
| `0x67` | constant zero (high form) |
| `0x68..0x77` | special values with no stable role; not usable as a register; read 0 |
| `0x78..0x7a` | `pi`, `e`, `1/(2*pi)` — a second copy of the tail of `0xf0..0xfa` |
| `0x7b` | special value with no stable role; reads 0 |
| `0x7c..0x7f` | lane-mask registers `p0..p3` |
| `0x80` | the condition register SCC |
| `0x81..0x9f` | special values with no stable role; read 0 |
| `0xa0..0xdf` | integer constants 1 to 64, encoded as `0x9f + n` |
| `0xe0..0xef` | integer constants -1 to -16, encoded as `0xdf + n` |
| `0xf0..0xfa` | 0.5, -0.5, 1.0, -1.0, 2.0, -2.0, 4.0, -4.0, pi, e, 1/(2*pi) |
| `0xfb..0xff` | special values with no stable role; read 0 |

The "no stable role" ranges have no established ABI role. They are not ordinary
scalar registers, must not enter a scalar allocation pool, and a decoder should
reject them. A hardware sweep of the whole byte range (`v_mov.b32` and
`s_mov.b32`, PS carrier) reads 0 from every one of them, so a zero read from an
unmodelled selector is the default outcome and carries no information about that
selector in particular.

`0x78..0x7a` is the exception among the unmodelled bytes and is measured, not
inferred: the hardware reads the same `pi`, `e` and `1/(2*pi)` bit patterns from
it as from `0xf8..0xfa`, while `0x70..0x77` reads 0. Whatever duplicates those
three constants into `0x78..0x7a` is `UNRESOLVED`; it is not a plain
"ignore bit 7", because that would also expose `0xf0..0xf7` at `0x70..0x77`.

`0xff` was previously listed as `constant zero`. The sweep reads 0 from it, but
that is the same value every unmodelled selector returns, so the read alone does
not establish `0xff` as a distinct zero source; whether it has an architectural
meaning of its own is `UNRESOLVED`.

Every reading above is an observation of one 8-bit field on the PS carrier
through two machine formats (SOP `GJ8` and VOP `GJ11`), which agree byte for
byte on all 151 deterministic encodings. It is not a statement about other
fields, other stages, or the 11-bit selector's high bits, which were not swept.

## The 11-bit selector

Vector ALU sources use an 11-bit selector:

```text
selector[10:0] = { namespace[2:0], base[7:0] }
GJ11 = { INST_HI[2:0], INST_LO[7:0] }
GK11 = { INST_HI[5:3], INST_LO[15:8] }
GA11 = { INST_HI[8:6], INST_LO[23:16] }
```

| Bit | Meaning |
|----:|---------|
| `base[7:0]` | the base selector from the table above |
| `0x100` | select a vector register: the source is `v[base]` |
| `0x200` | clear the sign bit of the source value |
| `0x400` | flip the sign bit of the source value |

The modifier bits apply to the value, not to the register: `0x600` is
`-abs(x)`.  They are per-source, and distinct from the per-instruction
`M.mod2` and `M.clmp1` modifiers.

Measured on hardware (PS carrier, `v_mov.b32 p0,v9,<selector>` with `v8`
preloaded to `-1.0f`): all three bits behave as tabulated, and the two sign
bits compose in the order clear-then-flip, which is what makes `0x600` equal
`-abs(x)` rather than a no-op.  `0x600` on `0` reads `0x80000000` and `0x600`
on `-1.0f` reads `-1.0f`, so the pair is a sign *set*, not a toggle of a
toggle.  The modifiers apply equally to a register, an inline constant, SCC and
a lane mask, so they really do act on the value and not on the operand kind.
`0x1f3` reading `v243` confirms the base index spans the full eight bits.

## Register files

The register files and everything a compiler must respect about them — the file
sizes, the reserved window at the bottom of the scalar file, the registers a
launch supplies, and the per-stage size rules — are in
[Registers and Allocation](../shader/registers.md).  Two facts belong here
because they are operand encoding rather than allocation:

* the lane-mask registers `p0..p3` have the fixed selectors `0x7c..0x7f`, and the
  condition register SCC the selector `0x80`, so both are reachable from any
  general-source position;
* a lane mask holds one bit per lane and a wave is 32 lanes wide, so one lane-mask
  register covers exactly one wave.

## Conditional execution

The predicate field `PJ2` and the lane-mask mechanism are two different things,
and both are present in the instruction set.

A lane mask in a register is the mechanism the established scalar and vector ALU
producers use to make a result conditional on which lanes are active, and it is
the mechanism behind the two hazards below.  Separately, several rows — the
emit forms, the interpolation forms and the matrix forms — carry their predicate
in the `PJ2` operand, where it gates whether the lane participates in the
operation.  The ALU producers write zero in that field, so its exact semantics
are not established, but a reader must not conclude that the field is unused by
the instruction set as a whole.

The lane-mask mechanism is:

* `p0..p3` (selectors `0x7c..0x7f`) are lane-mask registers.  `p0` is the
  active mask: it holds the lanes that are still alive for this invocation.
* The mask can also live in `s0..s3`, which are ordinary allocatable registers
  used as lane-mask values.
* A shader opens by installing a full-quad mask with the whole-quad-mask
  instruction, and restores the previous mask on exit.
* A lane is disabled by clearing its bit: logically AND the mask with the
  complement of the kill mask, or write an all-zero mask to end the whole
  invocation.
* Vector selection between two values uses a three-source vector select whose
  third source is a lane mask.
* A scalar condition is the SCC bit: compare instructions write it, and
  add-with-carry, subtract-with-borrow and scalar select read it back as their
  third source.
* Control flow branches on the condition register with a conditional branch;
  the register is encoded in `INST_HI[7:0]` and the displacement in
  `INST_LO[31:0]`.

Three hazards follow from that design and a producer must respect them:

1. **A lane mask does not survive a vector instruction.**  A mask must be folded
   into the data as a vector value, or consumed by a single broadcasted
   predicate, rather than kept live across vector ALU operations.
2. **Vector memory and sampling instructions clobber lane masks.**  A mask that
   must survive a load, store or sample has to be saved and restored around it.
3. **SCC is short-lived.**  Consecutive compare instructions can be read stale;
   a consumer of the condition must be materialised before the next compare is
   issued, and a compare whose result is a scalar register (rather than SCC) is
   not the established form.

Because masks are per-lane data, a divergent branch is expressed as a compare
into a mask, a branch, and mask manipulation, not as an instruction-level
predicate.

## Immediate forms

| Form | Width | Use |
|------|------:|-----|
| `IJ32`, `IK32` | 32 | literal bit pattern in the low word |
| `IA16`, `IJ16`, `IK16` | 16 | identifier, channel or offset literal |
| `UO8` | 8 | unsigned offset |
| `UO12` | 12 | unsigned offset (buffer addressing) |
| `UO20` | 20 | unsigned offset (scalar memory addressing) |

## Modifiers

A modifier is not one kind of thing.  Five kinds occur, they are distinguished
by what they act on rather than by what they are called, and the distinction is
the difference between a reader implementing the field correctly and guessing:

| Kind | Acts on | Members |
|------|---------|---------|
| `enable` | whether another operand contributes at all | `M.offen1`, `M.idxen1` |
| `width` | how many registers **another** operand occupies | `M.rsz1` |
| `form` | which instruction this is; fills no operand slot | `M.na1` |
| `value` | its own immediate operand slot | `M.dmask4`, `M.un1`, `M.bl11`, `M.bl21`, `M.mod2`, `M.clmp1` |
| `marker` | records a property of the instruction | `M.vm1` |

Only `value` behaves the way the word "modifier" suggests.  The others look
identical in an encoding table and are not interchangeable:

* An **`enable`** bit changes the instruction's *operand count*.  Clearing it
  does not set an operand to zero - it removes the operand, and the printed form
  loses a slot.  A reader who zero-fills instead of omitting encodes a different
  instruction.
* A **`width`** bit changes a *different operand's* size, so the register tuple
  in the same encoding is read differently depending on a bit elsewhere.  This
  is the `rsz` case below and it is the one most likely to corrupt a descriptor.
* A **`form`** bit is not an operand at all, so it must not appear in an operand
  model, and a disassembler that tries to print it will fail rather than print
  something wrong.
* A **`value`** bit is an ordinary immediate and a decoder may print it freely.

| Field | Bits | Kind | Default | Meaning |
|-------|-----:|------|---------|---------|
| `M.mod2` | 2 | `value` | 0 | vector operation mode |
| `M.clmp1` | 1 | `value` | 0 | clamp the result |
| `M.offen1` | 1 | `enable` | 0 | add the offset operand |
| `M.idxen1` | 1 | `enable` | 0 | add the index operand |
| `M.bl11`, `M.bl21` | 1 each | `value` | 0 | memory behaviour controls |
| `M.un1` | 1 | `value` | 0 | unnormalized texture coordinates |
| `M.na1` | 1 | `form` | 0 | array image |
| `M.rsz1` | 1 | `width` | 0 | resource tuple width: selects `rw8`/`s[0:7]` or `rw4`/`s[0:3]` |
| `M.dmask4` | 4 | `value` | 0 | texture channel mask |
| `M.vm1` | 1 | `marker` | 1 | control instruction flag |
| `M.sioid1` | 1 | `value` | 0 | SIO stream selector; 0 and 1 are both real streams, so a zero here is a choice and not an absence |
| `M.channum12` | 12 | `value` | 0 | SIO window size in channels |
| `M.chan12` | 12 | `value` | 0 | SIO channel slot |

The last three are described with the stream interface in
[the SIO chapter](../shader/sio.md); they are listed here because they are
modifier fields by encoding and it is the `value` kind, so they print at zero.
That is the one place where the "omit a clear modifier" rule must not be
applied: `sio:0` and `chan:0` name stream 0 and channel 0.

The default column is the value every established producer emits, which is the
only evidence there is for it; the hardware behaviour of a cleared `marker`, or
of a `form` bit set to the opposite value, is UNRESOLVED.

The `value` members are grouped by evidence, not by assumption.  Every texture
instruction the vendor backend models carries exactly **five** immediate
operands, and they are `dmask`, `un`, `rsz`, `bl1` and `bl2`; `mod2` and `clmp1`
are the corresponding pair on the vector rows.  Occupying an operand slot is
therefore a fact about them.

The vendor's five are not all the same kind, which is the point of the first
table: four of them - `dmask`, `un`, `bl1`, `bl2` - are `value` bits, and `rsz`
is not, despite sitting in the same list and printing like the others.  A reader
who takes the vendor's five-immediate list as a statement that all five are
ordinary operands will size the resource tuple from the wrong bit.

The exact arithmetic of `mod2`, `clmp1` and the `bl1`/`bl2` pair is
UNRESOLVED beyond their names; they are emitted as fixed values by every
established producer.

The texture instructions take their resource as a register tuple whose width is
part of the operand, not a property of the mnemonic: with `M.rsz1` clear the
resource occupies eight dwords (`rw8`, `s[0:7]`), with it set four (`rw4`,
`s[0:3]`).  Measured by flipping that one bit on the seeds of `gather`,
`gather.c.lz` and `samp.c.lz.o`, all three of which move `s[0:7] rw8` to
`s[0:3] rw4` and back.  A reader who takes the bit for a "result size" will
write the wrong resource length and read four scalar registers too many or too
few.

`M.dmask4`, `M.un1`, `M.bl11` and `M.bl21` are confirmed the same way: setting
each one alone appends `dmask(n)`, `un`, `bl1` and `bl2` to the disassembly and
changes nothing else.  `M.na1` is the exception: setting it alone makes the
vendor disassembler refuse the whole instruction rather than print an `na`
operand, so whether the bit is merely unmodelled there or means something else
is UNRESOLVED.

`M.na1` is the one modifier whose *kind* is a form bit rather than an operand, and
that is what explains its behaviour instead of leaving it as a curiosity: it is
the only bit in the layout that stops the instruction decoding when it is
flipped.  That the bit is not an operand is a property of the vendor compiler's
operand model; whether it switches between forms, and what it switches to, is a
hardware question this reference leaves open.

## Addressing families

The memory and image instructions use five operand shapes.

### Vector buffer

```text
RK8      descriptor base (or the architectural zero source 0x66)
RJ8x4    four-register buffer descriptor
VQ8      vector register holding the byte offset (or 0x66 when unused)
VD8      destination tuple
UO12     constant offset
M.offen1, M.idxen1   enable the offset and index contributions
M.bl11   set for the buffer forms
```

The address is `descriptor base + (offset when enabled) + (index when
enabled)`.  Atomic forms use the architectural zero source in the index
position and carry the scalar offset in `RK8`.

### Flat

```text
VJ8x2 / VK8x2   the absolute 64-bit address as an aligned vector register pair
RK8x2           a zero scalar pair
VA8             compare or RMW data
VD8             destination
M.bl11, M.bl21  memory behaviour controls
```

Flat addressing is a raw 64-bit address, not a descriptor-relative one.

### Texture and image

```text
VJ8x8   window of vector registers holding the coordinate tuple
RK8x8 / RK9x8   image descriptor (eight scalar registers)
RA8x4   sampler descriptor (four scalar registers)
VD8x4   four-register result
X0_4    channel mask
M.un1, M.na1, M.rsz1, M.bl11, M.bl21
```

The window is an upper bound: each variant consumes a prefix of it, and the
consumed length depends on the coordinate tuple of that variant.

### Shared memory

```text
VK8, VJ8, VA8, VD8   vector registers
X0_16                byte or element offset
```

### Scalar memory

```text
RJ8x2 / RJ8x4   address pair or descriptor
UO20            constant offset
GK8             the architectural zero source
RD8             result tuple
```

## UNRESOLVED

* The semantics of `PJ2`.
* The role of the special-value selector ranges `0x68..0x7b`, `0x81..0x9f` and
  `0xfb..0xfe`; they have observable values but no established ABI meaning.
* Whether the constant-zero selector and its high form are equivalent in
  hardware.
* Whether the launch supplies any value beyond the entry registers listed
  above; no other register is documented as pre-loaded.
* The exact semantics of the operation modifiers - **the reachable half is now
  separated from the unreachable one**.  `bl1` and `bl2` ARE reachable: they are the
  two boolean parameters of the buffer forms in the vendor's producer, ordered `bl2`
  first and then `bl1` (the closed intrinsic definition says so), and compiling the
  four combinations emits exactly `(none)`, `bl1`, `bl2` and `bl2, bl1` - so the
  identity and the order are established even though what the bits MEAN is not.
  `mod2` and `clmp1` are NOT reachable at all: neither appears anywhere in the
  producer's intrinsic layer, which is why every established producer emits them at
  fixed values.  That is a stronger statement than "changing one is unsupported" -
  there is no producer-level way to change one.
* What `bl1` and `bl2` MEAN.  One data point, from compiling the device library:
  the only buffer accesses in its compiled form are the compiler's own register
  spills, and `store.v.buff.b32 ... , bl1` / `load.v.buff.b32 ... , bl1` is how they
  are emitted - so `bl1` accompanies private scratch traffic.  That is a hint, not an
  answer.  The ordering experiment that would answer it is blocked twice over: the
  atomic implementations live in `opencl.bc`, which is generic (it contains no lggpu
  intrinsics until the runtime links it) and on which `llc -march=lggpu` CRASHES
  outright, while `ockl.bc` compiles cleanly (79,305 lines) and holds no atomics at
  all.
* What the hardware does when a modifier is not at its default: a cleared
  `marker`, or a `form` bit at the value the producers do not emit.  The
  defaults are observations of every established producer, not hardware
  behaviour.
* Whether any modifier other than `na` is a `form` bit.  The evidence that
  separates `enable` from `value` - flipping one bit and seeing whether an
  operand appears or a value changes - has been applied to `rsz`, `na`, `dmask`,
  `un`, `bl1` and `bl2`, and not exhaustively to the rest.
* The address unit and lane mapping of shared-memory addressing.
* The operand form of 8- and 16-bit buffer loads and stores: no verified form
  exists.
