# Blending

The output merger combines the fragment colour with the destination surface.
Blending is controlled by a per-draw control word and one blend word per render
target. This chapter defines the fields, the equations and the factors.

## Blend Control

Blend Control is 4 bytes at state offset `0x33c`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Independent enable | Honour each target's blend word independently. |
| `[1]` | Dual source | Enable dual-source blending. |
| `[3]` | Logic operation enable | Enable the logic operation. |
| `[7:4]` | Logic operation | Logic operation selector. |

When independent enable is clear, the first target's blend word applies to every
attachment.

## Per-target blend word

This word is the Blend RT record.

Each target has a 4-byte word at `0x340 + 4 * n`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Blend enable | Blend this target. |
| `[3:1]` | RGB equation | Blend equation for colour. |
| `[8:4]` | RGB source factor | Source factor for colour. |
| `[13:9]` | RGB destination factor | Destination factor for colour. |
| `[16:14]` | Alpha equation | Blend equation for alpha. |
| `[21:17]` | Alpha source factor | Source factor for alpha. |
| `[26:22]` | Alpha destination factor | Destination factor for alpha. |
| `[30:27]` | Colour mask | R, G, B and A write enables. |

## Equations

| Value | Equation |
|------:|----------|
| 0 | Add: source * source factor + destination * destination factor. |
| 1 | Subtract: source * source factor - destination * destination factor. |
| 2 | Reverse subtract: destination * destination factor - source * source factor. |
| 3 | Minimum. |
| 4 | Maximum. |

## Factors

| Value | Factor |
|------:|--------|
| 1 | One |
| 2 | Source colour |
| 3 | Source alpha |
| 4 | Destination alpha |
| 5 | Destination colour |
| 6 | Source alpha saturate |
| 7 | Constant colour |
| 8 | Constant alpha |
| 9 | Source 1 colour |
| 10 | Source 1 alpha |
| 17 | Zero |
| 18 | One minus source colour |
| 19 | One minus source alpha |
| 20 | One minus destination alpha |
| 21 | One minus destination colour |
| 23 | One minus constant colour |
| 24 | One minus constant alpha |
| 25 | One minus source 1 colour |
| 26 | One minus source 1 alpha |

The values 11 through 16, 22 and 27 through 31 are not assigned by the encoding.

## Logic operations

| Value | Operation |
|------:|-----------|
| 0 | Clear |
| 1 | Nor |
| 2 | And inverted |
| 3 | Copy inverted |
| 4 | And reverse |
| 5 | Invert |
| 6 | Xor |
| 7 | Nand |
| 8 | And |
| 9 | Equivalent |
| 10 | Noop |
| 11 | Or inverted |
| 12 | Copy |
| 13 | Or reverse |
| 14 | Or |
| 15 | Set |

## Blend constants

Four float patterns at `0x360` supply the constant colour and constant alpha.

| Bits | Field | Meaning |
|------|-------|---------|
| `[31:0]` | Red bits | Red float pattern. |
| `[63:32]` | Green bits | Green float pattern. |
| `[95:64]` | Blue bits | Blue float pattern. |
| `[127:96]` | Alpha bits | Alpha float pattern. |

## Rules

- A colour attachment with a pure integer format cannot be blended. It also
  cannot use a logic operation or alpha-to-coverage.
- Alpha-to-coverage requires 4x multisampling.
- A colour write mask of zero is not a no-op on the depth and stencil path: a
  fully masked colour write can suppress the fragment before depth and stencil
  commit. A host that needs the fragment to run for its depth effect SHOULD use a
  blend equation that preserves the destination rather than a zero write mask.

> UNRESOLVED: which factors are legal in dual-source mode, and how a logic
> operation behaves when several render targets are bound.

## Host policy

The current driver swaps the green and blue blend constants on float targets
before writing them. The swap compensates for the channel order of the surface
format; it is a driver convention, not a hardware requirement.
