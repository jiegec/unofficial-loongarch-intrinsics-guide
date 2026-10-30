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
| Predicate | `PJ2` | present in most layouts; it is the instruction mask, and an established producer uses all four (p0-p3) as the leading operand |

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

`PJ2` is the instruction's mask selector, and it is a first-class part of the
execution model rather than a per-family oddity.  The field is 2 bits (the
position differs by format: `HI[13:12]` in `vop`, `HI[17:16]` in `vop-imm`,
`smem`, `flat`, `tex-buffer` and `shared-memory`, `HI[21:20]` in `sio-id`,
`HI[17:16]` in `jump-stack`) and it names one of the four lane-mask registers
`p0..p3`; the instruction then executes only on the lanes whose bit is set in
that register.  A census of the closed producer's whole linked program counts
all four in the field (`p0` 2772, `p1` 5244, `p2` 4290, `p3` 4142), so selecting
a mask other than `p0` is ordinary, not a corner case.

The behaviour was measured on the compute pipe (32 lanes) on 2026-10-21 with
the sequence runner; each row is one submission:

| Setup | Observation |
|-------|-------------|
| `p1=0x55555555`, `v_mov` under `PJ2=1` | only lanes 0,2,...,30 were written |
| `p2=0x55555555`, `PJ2=2`; `p3=0x55555555`, `PJ2=3` | same lane set each time |
| `p0=0xffff0000`, `p1=0x55555555`, `PJ2=1` | lanes 0..15 written too: the mask is an **absolute** lane set, not an intersection with `p0` |
| `p0=0`, `p1=all`, flat store under `PJ2=1` | the store landed |
| `p0=all`, `p1=0`, flat store under `PJ2=1` | the store did not land |
| `p1=0x55555555`, `v_mov` under `PJ2=1`, then read `p1` | still `0x55555555` |
| `p1=0x55555555`, flat store under `PJ2=1`, then read `p1` | still `0x55555555` |
| `v_cmp_eq_u32` with `p2` as the destination | `p2` became `0xffffffff` |
| `p2=0`, `p1=0x55555555`, `v_cmp_eq_u32` under `PJ2=1` writing `p2` | `p2=0x55555555`: the destination write is gated, so a zeroed destination plus a gated compare yields `parent & condition` in one instruction |
| `cb p1, +1` with `p1=0` / `p1=0xffffffff` | not taken / taken |

The rest of the model was measured in the same campaign (2026-10-21/22);
each row is again one submission:

| Setup | Observation |
|-------|-------------|
| immediate-form `v_add_imm_u32` under `PJ2=1`, `p1=0x55555555` | only the even lanes were written: the **vop-imm** layout gates like `vop` |
| `store_sm_b32` under `PJ2=1`, cells pre-filled per lane | only the selected lanes' cells changed: the **shared-memory** layout gates |
| `load_sm_b32` under `PJ2=1`, destination pre-filled per lane | only the selected lanes' destinations changed |
| the same two instructions with `p1=0xaaaaaaaa` instead of `0x55555555` | the writing lanes flipped to the odd ones: the field names a register and that register's **contents** decide the lanes |
| `v_add_imm_u32` under `PJ2=2` with `p2=0x0000ffff`; `store_sm_b32` under `PJ2=3` with `p3=0xf0f0f0f0` | lanes 0..15; and cells 4..7, 12..15, 20..23, 28..31: any of the four registers works for these layouts |
| `store_sm_b32` under `PJ2=1` with `p1=0` while `p0=0xffffffff` | no cell changed: the selected register is used, `p0` does not leak in |
| the encoded words of the gated and ungated forms of the same instruction | identical except `INST_HI[16]`, the low bit of the `HI[17:16]` selector: the gating really is the field, not a side effect of another operand |
| gated shared-memory store, then read `p1` | still `0x55555555`: the mask survives a shared-memory access |
| `czb p1, +1` with `p1=0` / `p1=0x55555555` | taken / not taken: `czb` reads a mask register, as `cb` does |
| `s_wqm_b32 p2, p1` with `p1=1`; in place on `p1`; in place on `p0` | `p2 = 0xf` / `p1 = 0xf` / `p0 = 0xf`: the whole-quad transform takes and returns mask registers, including the live set |
| `p1=0x55555555`, `v_cmp_lt_u32` under `PJ2=1` **writing `p1`**, condition true in lanes 0..3 | `p1 = 0x5` = old & condition: the selector is read before the gated write, so selecting and overwriting one mask is safe |
| `p1..p3` written with markers, `s4..s11` staged with eight distinct patterns, both read back | neither direction changed: the mask file is **its own** file, not a window of the scalar file |
| destination filled with `0xdeadbeef` (then with the lane id) in every lane, then a gated `v_mov` / gated load of another value | the unselected lanes kept `0xdeadbeef` (and the lane id): a gated write **suppresses** the write, it does not clear the lane |
| destination `0xdeadbeef`, first copy `0xaaaaaaaa` under `p1=0x55555555`, second copy `0xbbbbbbbb` under the complement | even lanes `0xaaaaaaaa`, odd lanes `0xbbbbbbbb`: two masked copies partition the wave and do **not** erase each other |

The fragment pipe (4x4 = 16 lanes) answers the same questions the same way, which
is what makes the mask model usable for a fragment stage that needs helper lanes:

| Setup | Observation |
|-------|-------------|
| `p1=1`, `s_wqm_b32 p2, p1`; in place on `p1`; narrow `p0` to 1 then in place on `p0` | `0xf` each time: the whole-quad transform works on the fragment stage |
| `p0` narrowed to a single lane, then a compare whose destination write is gated | the destination read back `0x1`: a narrowed `p0` really gates the fragment stage's vector writes |
| `p1=0x5555`, gated fragment store, then read `p1` | the store landed and `p1` still read `0x5555` |
| whole-quad round trip: narrow `p0` to one lane, `s_wqm_b32 p0,p0`, gated compare, restore the exact mask, gated compare | the compare under the widened mask read `0xf` (helper lanes ran), the one under the restored mask read `0x1` |

So: a compare can produce a mask directly in a mask register, and a compare
executed under `PJ2=k` writes only `pK`'s lanes — with a zeroed destination that
is exactly `pK & condition`.  A branch can read a mask register directly, and the
mask selected by an instruction survives that instruction,
including a vector memory instruction.  A gated instruction suppresses the write,
so a lane outside the selected mask keeps its previous value; it does not clear
it, and two gated writes with complementary masks compose instead of erasing each
other.  `p0` is still the invocation's live set
by convention — the established producers open with the whole-quad mask in `p0`
and treat the others as region masks — but nothing in the encoding or in the
measured behaviour gives `p0` a privilege over `p1..p3`.

Three consequences for a producer:

* `p1..p3` are **undefined at launch** (each submission observed a different
  residue), so a shader must write a mask before selecting it.
* Because masks are absolute, the compiler must compute `parent & condition`
  when it opens a region instead of inheriting it from a single exec register.
* Because a gated write suppresses rather than clears, a per-lane merge can be
  built from one gated copy per predecessor mask (as long as the masks cover the
  wave), and a whole-quad region only has to widen and then restore the live set
  — helper lanes never have to be cleaned up out of a value.

The `p0..p3` mechanism is:

The lane-mask mechanism is:

* `p0..p3` (selectors `0x7c..0x7f`) are lane-mask registers and any of them can
  gate an instruction.  `p0` holds the lanes still alive for the invocation;
  `p1..p3` hold the region masks the producer derives from it.
* The mask can also live in `s0..s3`, which are ordinary allocatable registers
  used as lane-mask values.
* A shader opens by installing a full-quad mask with the whole-quad-mask
  instruction, and restores the previous mask on exit.
* A lane is disabled by clearing its bit: logically AND the mask with the
  complement of the kill mask, or write an all-zero mask to end the whole
  invocation.
* Vector selection between two values uses a three-source vector select whose
  third source is a lane mask.

* The dot-product family carries a source the driver row does not name. A
  `v_dp4a_*` row is `GJ11 GK11 VD8`, but the encoding also carries `LO[23:16]`,
  outside every field the row names - the driver prints it as ignored - and the
  hardware adds the scalar register that field selects. Measured on the compute pipe:
  every case returned expected + `0x000b0000`, and that is exactly the `s0` the
  prelude stages as the result-store descriptor base. On graphics, where the packet
  hydrates `s0` to zero, the term is absent.

  That scalar is the **whole** implicit input.  The one further implicit term this
  page used to leave open - a `0xff` byte pattern at every lane count - is not a
  property of the row: with the carrier zeroing the selected scalar, all four
  `v_dp4a_*` forms pass their case vectors on the compute pipe (103/103 each,
  `code/lg200/gpu/README.md`), and the `0xff` reading belonged to `v_mbcnt_u32` on
  the graphics carrier (0x103 where graphics reads 0x1ff, at every lane count) and
  does not reproduce on the compute carrier (41/41 there, 2026-10-30).  So it says
  that the carriers stage lanes/exec differently, not that the instruction has a
  second hidden addend.
* A scalar condition is the SCC bit: compare instructions write it, and
  add-with-carry, subtract-with-borrow and scalar select read it back as their
  third source.
* Control flow branches on the condition register with a conditional branch;
  the register is encoded in `INST_HI[7:0]` and the displacement in
  `INST_LO[31:0]`.

One hazard survives from the earlier reading of this section:

1. **SCC is short-lived.**  Consecutive compare instructions can be read stale;
   a consumer of the condition must be materialised before the next compare is
   issued, and a compare whose result is a scalar register (rather than SCC) is
   not the established form.

A lane mask register value written into
`p0`-`p3` stays there until an instruction writes that mask again, so a compare
result can be consumed many instructions later.  

Because masks are per-lane data but the branch is wave-wide, a divergent branch
is expressed as a compare into a mask, a branch on that mask, and mask
manipulation.  It is not an instruction-level predicate: `PJ2` selects the
mask, it does not evaluate a condition.

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

The exact arithmetic of `mod2` and `clmp1` is UNRESOLVED beyond their names - and the
reason is stronger than "every producer emits fixed values": neither appears anywhere
in the producer's intrinsic layer, and in two whole linked programs (`30,000 lines
each, runtime libraries included) the disassembler prints no `mod2`, `clmp1`, `dmask`
or `un` at all, which means the producer emits all of them at ZERO.  The `bl1`/`bl2`
pair is a different case and is now separated out: both ARE reachable, as the two
boolean parameters of the buffer forms (ordered `bl2` first, then `bl1`), and the only
use the producer makes of them is `bl1` on its private spill/reload traffic - 26
occurrences and no `bl2`.  So what remains open is not their identity but what the
bits mean to the hardware.

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

* The `PJ2` selector itself is settled (see [Conditional execution](#conditional-execution));
  what stays open is narrower: the fragment pipe (WQM/helper lanes), the sampling and
  shared-memory forms, and whether an instruction that both selects a mask and writes
  that same register reads the old or the new value back.  The launch values of
  `p1..p3` are undefined by measurement, not by documentation, and the whole-quad-mask
  interaction with a region mask is untested.
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
* What `bl1` and `bl2` MEAN - **narrowed by a full pipeline run**, not by a hint.
  Linking a probe against the runtime's own bitcode (`opencl.bc` + `ockl.bc` + the
  `oclc_*` modules) and compiling the result with `llc -march=lggpu` gives a whole
  program, and the census is unambiguous: it contains 26 buffer accesses, every one
  of them `load.v.buff.b32` / `store.v.buff.b32` on the compiler's **private
  spill/reload** path, and every one carries `bl1` - while `bl2` appears NOWHERE in
  the program.  So within everything a producer emits, `bl1` marks real private
  scratch traffic and `bl2` is simply not used; what either bit means to the hardware
  is still open.
* A lowering fact from the same run, useful on its own: **the producer does not use
  the buffer forms for OpenCL atomics at all**.  Global atomics lower to `flat`
  (241 in that program) and local ones to `sm` (3), and the buffer atomics appear
  zero times.  So `atomic_v_buff_*` is not the path a working OpenCL compiler takes
  for `atomic_*` on this target.
* What the hardware does when a modifier is not at its default: a cleared
  `marker`, or a `form` bit at the value the producers do not emit.  The
  defaults are observations of every established producer, not hardware
  behaviour.
* Whether any modifier other than `na` is a `form` bit.  The evidence that
  separates `enable` from `value` - flipping one bit and seeing whether an
  operand appears or a value changes - has been applied to `rsz`, `na`, `dmask`,
  `un`, `bl1` and `bl2`, and not exhaustively to the rest.
* The address unit and lane mapping of shared-memory addressing.
* The operand form of 8- and 16-bit buffer loads and stores - **settled 2026-10-13, and
  this bullet was the last place still calling it open**.  The form IS the layout: the
  8-bit loads carry `RK8`, `RJ8x4`, `VD8`, `UO12` and the enables with no index operand,
  while the 16-bit loads and the 8-bit store name one (`VQ8` and `RA8`).  What is missing
  is execution, not description: the carrier cannot stage a buffer descriptor, so no
  sub-word buffer access has ever been run.
* The alignment rule for a two-register pair - **narrowed by the producer's output**, not
  decided.  Across two whole linked programs (`30,000 lines each, runtime libraries
  included) every multi-register vector operand starts at an even register: 5,481
  occurrences drawing on 21 distinct bases, and not one odd base.  That is usage evidence
  and not a requirement - the allocator may simply never produce an odd base - so the
  question stays open and one deliberate odd-base submission would settle it.  What the
  evidence does rule out is the opposite reading: nothing shows an odd base being used
  and working, so nothing contradicts a rule that pairs are even-aligned.
