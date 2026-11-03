# Blending

The output merger combines the fragment colour with the colour already in the
attachment. LG200 does that from a 4-byte control word, one 4-byte word per
colour target and four constant words. Each target's word holds its equation, its
four factors and its write mask; the control word chooses between that per-target
mode and one shared configuration, and can replace blending with a bitwise
operation for the whole draw.

## Blend Control

Blend Control is 4 bytes at state offset `0x33c`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Independent enable | Honour each target's blend word separately; clear, target 0's word applies to every attachment. |
| `[1]` | Dual source | The fragment shader supplies a second colour, read by the Source 1 factors. |
| `[3]` | Logic operation enable | Replace blending with the bitwise operation in `[7:4]`. |
| `[7:4]` | Logic operation | One of the sixteen operations listed below. |

Bit 2 carries no field; a producer MUST write it as zero. Set, each of the eight
per-target words is read on its own; clear, every attachment reads target 0's
word, colour mask included, so attachments that need different masks or
equations MUST set the bit. A draw that pairs a null attachment with a real one
needs the bit as well: the null slot must hold a zero mask while the real slot
writes.

## Per-target blend word

Each colour target has one 4-byte Blend RT record; target *n* uses `0x340 + 4n`
for *n* in 0–7, and the eight records end where the constant block begins. How
many of them a draw uses is its MRT count ([Draw](draw.md#draw-info)).

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Blend enable | Apply the equation to this target. Clear, the fragment colour replaces the destination through the colour mask. |
| `[3:1]` | RGB equation | Equation for the colour channels. |
| `[8:4]` | RGB source factor | Factor applied to the fragment colour. |
| `[13:9]` | RGB destination factor | Factor applied to the destination colour. |
| `[16:14]` | Alpha equation | Equation for the alpha channel. |
| `[21:17]` | Alpha source factor | Factor applied to the fragment alpha. |
| `[26:22]` | Alpha destination factor | Factor applied to the destination alpha. |
| `[30:27]` | Colour mask | Write enables: bit 27 red, bit 28 green, bit 29 blue, bit 30 alpha. Bit 31 carries no field. |

No other block of the object carries a colour mask. The state block persists between records
([State records](state-records.md#semantics)), so a slot a draw does not rewrite
keeps the previous word: a producer MUST write a disabled record — enable clear,
mask zero — into every slot the draw does not use.

## Equations

| Value | Equation |
|------:|----------|
| 0 | Add: source × source factor + destination × destination factor. |
| 1 | Subtract: source × source factor − destination × destination factor. |
| 2 | Reverse subtract: destination × destination factor − source × source factor. |
| 3 | Minimum: the smaller of the source and destination values. |
| 4 | Maximum: the larger of the source and destination values. |

Colour and alpha carry separate equation fields and can differ within one word.
Values 5–7 have no assignment and a producer MUST NOT write them. A mode that
needs more than a source and a destination term has no encoding here, so a
producer MUST evaluate it in the fragment shader with blending disabled. Whether
Minimum and Maximum read the factor fields, or a result is clamped to the
attachment's range first, is UNRESOLVED.

## Factors

| Value | Factor | Meaning |
|------:|--------|---------|
| 1 | One | 1. |
| 2 | Source colour | The fragment colour. |
| 3 | Source alpha | The fragment alpha. |
| 4 | Destination alpha | The destination alpha. |
| 5 | Destination colour | The destination colour. |
| 6 | Source alpha saturate | Saturate of the fragment alpha; the exact clamp is UNRESOLVED. |
| 7 | Constant colour | The constant colour words. |
| 8 | Constant alpha | The constant alpha word. |
| 9 | Source 1 colour | The second fragment colour, dual-source mode only. |
| 10 | Source 1 alpha | The second fragment alpha, dual-source mode only. |
| 17 | Zero | 0. |
| 18 | One minus source colour | 1 − the fragment colour. |
| 19 | One minus source alpha | 1 − the fragment alpha. |
| 20 | One minus destination alpha | 1 − the destination alpha. |
| 21 | One minus destination colour | 1 − the destination colour. |
| 23 | One minus constant colour | 1 − the constant colour. |
| 24 | One minus constant alpha | 1 − the constant alpha. |
| 25 | One minus source 1 colour | 1 − the second fragment colour. |
| 26 | One minus source 1 alpha | 1 − the second fragment alpha. |

Values 1–10 name a factor and adding 16 to one of them names its complement, so
17 is the complement of one, that is zero. A producer MUST NOT write an unassigned
value — 0, 11–16, 22 and 27–31; 22 would be the complement of source alpha
saturate. What the unit does with an unassigned value is UNRESOLVED.

## Dual source

Dual source is bit 1 of Blend Control. The fragment shader writes a second colour
beside the first, and the second leaves through export slot 1, the slot attachment
1 would otherwise use, while the target count stays at one; a dual-source draw
therefore covers exactly one attachment, because with two the second colour would
collide with the second attachment's own export. Only four factors read that
colour — source 1 colour (9), source 1 alpha (10) and their complements (25, 26) —
so without the bit they name a colour the shader never exported: a producer MUST
set the bit whenever a factor field names a Source 1 factor, and a word that
names none does not need it. Both colours are exported in the target's own
format, so the second is never wider than the first.

## Logic operations

Bit 3 enables the operation and `[7:4]` selects it; it combines the fragment
value with the destination bit by bit.

| Value | Operation | Result per bit, *s* = source, *d* = destination |
|------:|-----------|--------------------------------------------------|
| 0 | Clear | 0 |
| 1 | Nor | not (*s* or *d*) |
| 2 | And inverted | *s* and not *d* |
| 3 | Copy inverted | not *s* |
| 4 | And reverse | not *s* and *d* |
| 5 | Invert | not *d* |
| 6 | Xor | *s* xor *d* |
| 7 | Nand | not (*s* and *d*) |
| 8 | And | *s* and *d* |
| 9 | Equivalent | not (*s* xor *d*) |
| 10 | Noop | *d* |
| 11 | Or inverted | not *s* or *d* |
| 12 | Copy | *s* |
| 13 | Or reverse | *s* or not *d* |
| 14 | Or | *s* or *d* |
| 15 | Set | all ones |

One operation covers the whole draw, with no per-target selection, and no
attachment blends while it runs, so a producer MUST clear every blend enable
rather than rely on the equation fields. The operation is bitwise, so the
destination contributes a bit pattern; how the unit forms one from a normalised or
floating-point attachment is UNRESOLVED, as is the result on an attachment whose
colour is exported as an integer, and the behaviour with several attachments
bound.

## Blend constants

Blend constants are 16 bytes at state offset `0x360`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[31:0]` | Red bits | Red constant, as a 32-bit float pattern. |
| `[63:32]` | Green bits | Green constant, as a 32-bit float pattern. |
| `[95:64]` | Blue bits | Blue constant, as a 32-bit float pattern. |
| `[127:96]` | Alpha bits | Alpha constant, as a 32-bit float pattern. |

Each word is an IEEE-754 binary32 pattern used as a value. One set serves the
whole draw: constant colour is the four-word value, constant alpha its alpha word,
and factors 7, 8, 23 and 24 are the only readers. The unit reads the words in the
attachment's channel order — red, green, blue, alpha for a normalised or legacy
target, red, blue, green, alpha for a floating-point target. On a floating-point
target a producer that wants the colour (r, g, b, a) MUST write the words as
r, b, g, a, or constant colour takes green from the blue word and blue from the
green word. The order follows the first attachment's format.

## Rules

- Blending is defined only for a normalised or floating-point colour value. An
  integer colour-buffer format does not blend at all, and the formats that have no
  colour-buffer representation of their own — 16-bit unorm, 8- and 16-bit snorm,
  packed 10-10-10-2 uint — are stored through an integer format and do not blend
  either. Whether the unit ignores such a blend word or applies it to raw integers
  is UNRESOLVED; a producer MUST NOT enable blending on any of them.
- A colour mask of zero on every target is not a no-op. When such a draw also
  writes depth or stencil, the fragment is discarded before those commits and the
  updates are lost. A producer that needs the depth or stencil update MUST instead
  blend with a full colour mask, a source factor of zero and a destination factor
  of one, which reproduces the destination; that substitution is itself a blend,
  so it MUST NOT be used on the integer path above, where it does not hold the
  destination and the fragment colour lands anyway.
- Alpha-to-coverage is not blend state: coverage is computed in the fragment
  shader and needs a 4x framebuffer
  ([Multisampling](multisampling.md#alpha-to-coverage)).

## Open questions

- The behaviour of an unassigned factor value, of a set bit 2 in Blend Control
  and of a set bit 31 in a blend word.
- Whether Minimum and Maximum read the factor fields, and whether a result is
  clamped to the attachment's range.
- Whether dual-source blending can cover more than one target, and which factors
  the unit accepts once the dual-source bit is set.
- How a logic operation behaves when several attachments are bound, and how the
  unit forms the bit patterns it combines.
