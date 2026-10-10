# Depth, Stencil and Blending

The depth, stencil and blend blocks are per-draw state inside the state object.
Field positions are in [The State Object](state-object.md); this chapter
describes what the fields mean and the rules that tie them to the attachments.

## Depth

| Field | Effect |
|-------|--------|
| depth enable | perform the depth test |
| depth write | write the resulting depth value |
| near/far clamp | clamp the incoming depth to the viewport's depth range instead of clipping it |
| compare function | never, less, equal, less-equal, greater, not-equal, greater-equal, always |
| polygon offset units, factor, clamp | bias the depth value of filled, lined or pointed primitives |

The depth **range** belongs to the viewport record, not to this block: the Z
scale and offset place the incoming depth, and the record's depth minimum and
maximum bound it.

## Stencil

Each face has its own record, front at `0x334` and back at `0x338`:

| Field | Effect |
|-------|--------|
| enable | run the stencil test for this face |
| compare function | the same eight-value enumeration as depth |
| fail operation | applied when the stencil test fails |
| depth-pass operation | applied when both tests pass |
| depth-fail operation | applied when the stencil test passes but depth fails |
| value mask | bits compared against the reference |
| write mask | bits written back |
| reference value | the comparison reference, carried in the record |

Operations are keep, zero, replace, increment-clamp, decrement-clamp,
increment-wrap, decrement-wrap and invert.  Because the reference value lives in
the front record, a host that uses two-sided stencil must keep both records
consistent.

## Blending

The blend control word selects independent per-target blending, dual-source
blending and a logic operation, and carries the logic operation selector.  Each
render target has its own blend word:

| Field | Effect |
|-------|--------|
| enable | blend this target |
| RGB equation | add, subtract, reverse-subtract, minimum, maximum |
| RGB source and destination factors | 5 bits each |
| alpha equation | the same equation enumeration |
| alpha source and destination factors | 5 bits each |
| write mask | which of R, G, B and A are written |

When independent blending is clear, the first target's blend word applies to
every attachment, so a host must not assume per-target words are honoured
without enabling the mode.

Blend constants are four float patterns and are the operands of the
constant-colour and constant-alpha factors.

Two rules constrain the combination of state and attachments:

* A colour attachment in a pure integer format cannot be blended, and cannot use
  a logic operation or alpha-to-coverage.
* Alpha-to-coverage requires 4x multisampling; a draw that requests it with any
  other sample count must be rejected.

## Multisampling and resolve

A 4x colour resolve runs as a GPU pass that reads the sample plane.  Depth and
stencil resolve run through CPU transfers that read the sample plane and write a
single plane.  A host therefore must not assume that a depth resolve is free or
that it can be folded into the same pass as the colour resolve.

Resolve of a multisample attachment does not use the blend path: it is a copy
between the sample plane and a single-sample surface.

## UNRESOLVED

* The complete blend-factor enumeration: the factor fields are known to be five
  bits wide and to cover the usual source, destination, constant and
  source-alpha-saturate categories, but the value of every factor is not
  established here.
* Which factors are legal in dual-source mode.
* The behaviour of a logic operation when several render targets are bound.
* Whether the near/far clamp applies to floating-point depth formats.
* The exact stencil wrap and clamp boundaries for the increment and decrement
  operations.
* How depth and stencil interact when only one of the two is bound, and the
  attachment combinations that are legal.
