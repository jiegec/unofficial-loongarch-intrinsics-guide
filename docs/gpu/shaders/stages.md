# Shader Stages

The graphics state block describes six shader slots and four shader roles. Each
slot holds one 96-byte shader-info record, and the shader config word decides
which slots run and which role each carries. This chapter covers the slots, the
roles, the selection, the entry and exit sequence and the agreements between
neighbouring stages; the record's fields, the launch registers and the channel
interface are in [Shader ABI](shader-abi.md), [Registers](registers.md) and
[SIO](io.md). A compute dispatch is not one of these slots; it launches on the
compute pipe and is in [Compute](compute.md).

## Slots

Six records of 96 bytes start at state offset `0x540`, at a stride of `0x60`:

| Slot | State offset | Shader type | What it holds |
|------|-------------:|------------:|---------------|
| VS | `0x540` | 3 | A copy of the vertex record |
| HS | `0x5a0` | — | Nothing |
| DS | `0x600` | 3 | The vertex role |
| GS | `0x660` | 4 | The geometry role |
| SS | `0x6c0` | 6 | The streamout kernel |
| PS | `0x720` | 5 | The fragment role |

The slot names are the shader-config enable names: VS vertex, HS hull, DS
domain, GS geometry, SS streamout, PS pixel. The type field names the role, not
the slot: the vertex program is a type 3 record installed in the domain slot,
selected by enabling that slot with the domain-as-vertex bit.

Two slots are not stages. The HS slot carries no stage: the encoding defines a
hull-stage record, but no state path writes one, so the slot stays zero. The VS
slot carries a second copy of the vertex record so that an instruction fetch
through it lands on mapped code — an all-zero record names address 0 through its
kernel-entry field, and no client mapping covers address 0. Only a record that
re-establishes the whole state block refreshes that copy, so a delta record
leaves the previous one in place; the slot is excluded from the per-draw
argument-base patch and from the delta write log for that reason.

When a cached state block starts a new batch, the VS copy MUST be rebuilt from
the current DS record after its argument roots have been relocated. A cache
entry created partway through an earlier batch can contain that batch's first
vertex program in the VS slot, even though its DS slot describes a later
program. Reusing the inherited copy would name code and descriptor roots that
the new submission does not retain. Retaining the copy within one batch and
refreshing it at the next batch boundary are separate requirements; neither a
cache hit nor an unchanged application pipeline transfers resource ownership
from an earlier submission.

The cached-state verifier therefore compares the VS copy when establishing a
fresh block. It excludes the copy only while a batch's established state is
still valid. The rule applies with and without a geometry stage and also to
independently built state blocks.

## Domain as vertex

The domain-stage config at `0x4d8` is the vertex role's config; no separate
vertex-stage record is written. It declares the record the stage produces
(position size, total size, attribute count, clip distance count, and whether an
edge flag, a point size or a layer value rides it) and whether the stage
consumes the per-draw instance id. The declared total size MUST equal the
channel count the stage exports; the two are the same compiler-reported width,
so a host that sizes the ring from another varying set leaves the consumer
reading a window the stage never wrote.

A geometry stage changes one thing: the vertex stage then issues no export and
the geometry stage reads the domain ring directly. The ring is sized from the
vertex stage's exported channel width, or from `(varyings + 2) * 4` channels
with a floor of 12 when no width is published.

## Stage roles

| Role | Slot | Receives | Produces |
|------|------|----------|----------|
| Vertex | DS | Vertex and instance ids in `v0` and `v1`, the attribute root, the descriptor roots | An SIO0 record |
| Geometry | GS | The domain ring, six 5-bit vertex selectors in `v0`, the primitive id in `v1` | An SIO1 record |
| Fragment | PS | Interpolated coefficient streams, a fragment input window, the entry mask in `p0` | An SIO1 colour and depth export |
| Streamout | SS | The read-write root, a buffer descriptor, a record count | Buffer writes |

A hull role does not exist: only vertex, geometry and fragment stages can be
created, and the primitive-type encoding has no patch entry, so a patch draw is
refused before any state is built.

The streamout role is not application code. Its record names a kernel that
copies per-vertex records the vertex stage left in the argument buffer into the
transform-feedback buffer, loading the descriptor through the read-write root.
The record carries type 6, an initial VGPR count of 2, a tail word of `0x20`,
and the record capacity in argument 2.

## Selection

The shader config word at `0x4c0` enables slots and selects modes; its bit
assignment is in [the state
object](../graphics/state-object.md#shader-config). Three shapes are
established:

| Pipeline | Enabled | Slots that carry a role |
|----------|---------|-------------------------|
| Vertex and fragment | DS enable, domain-as-vertex | DS, PS |
| Vertex, geometry, fragment | DS enable, GS enable, domain-as-vertex | DS, GS, PS |
| Either, with streamout | SS enable | SS, in addition to the row above |

Every slot that can carry a role is written on every draw, and a slot this draw
does not run is written as an all-zero record, so the previous draw's program
cannot survive in the persistent block. With a geometry stage the GS
primitive-type field names the output primitive as a category — points, lines or
triangles — rather than an assembly mode; the rest of the geometry config is in
[Geometry](geometry.md).

Four config bits are never set: the vertex enable (bit 0), the hull enable
(bit 1), "domain includes geometry" (bit 6) and "vertex includes hull" (bit 7).
What a launch does when the vertex role is enabled in the VS slot, and what
either fusion bit fuses, is UNRESOLVED.

## Entry and exit

A stage starts at the address in its record's kernel-entry field. There is no
entry stub: the field names the first instruction, and the code size MUST equal
the length of the image it belongs to.

The prologue is execution-mask setup. `p0` carries one bit per active lane at
launch and is copied to a scalar register; `p2` and `p3` are undefined at
entry, so a stage reads only `p0`. A stage that uses helper lanes keeps that
capture in a second register and expands the live mask to whole quads, so a
derivative sees every lane of a quad even when some are inactive.

The epilogue is fixed:

1. Write the stage's outputs.
2. If the mask was expanded for helper lanes, restore it by intersecting the
   running mask with the captured one, then commit the export: stream selector,
   channel count, and `p0` as the mask. Helper lanes contributed to the body but
   are not in the restored mask, so their results are not committed. The vertex
   role exports to SIO0, the geometry and fragment roles to SIO1.
3. Order the work the export depends on with the ordering instruction, which
   waits on all four domains unless placement assigns a narrower one.
4. Terminate the invocation with `exit`.

Three cases skip or shrink the export: a vertex stage followed by a geometry
stage commits none; a fragment stage that writes depth but no colour commits a
one-channel export, because a fragment it never commits is dropped before the
depth/stencil update; a stage that writes nothing ends at `exit`. A discard is
not an exit — it clears the killed lanes from the active mask and the invocation
continues, and since the export commits the mask, a lane removed from it writes
nothing.

## Consistency

| Agreement | What must match |
|-----------|-----------------|
| Kernel entry and code | The entry names an instruction in the uploaded image; the code size is that image's length |
| Record and role | The record's type is the role the enabled slot runs |
| SIO0 ring width | The vertex role's export channel count equals the domain-stage total size the consumer reads with |
| Attribute count | The exported varying count equals the consumer's declared attribute count |
| Fragment attribute map | Each entry's attribute id is the producer's output ordinal for that varying |
| Fragment input window | The coefficient count and the input-enable bits describe the streams the program reads |
| Addresses | The kernel entry, the constant-buffer root, the constant root and the resource root lie at or above the GPU virtual-address base |

The varying layout is checked on both edges when a pipeline is linked: vertex to
geometry — or vertex to fragment when no geometry stage runs — and then geometry
to fragment. For every varying the consumer reads, the producer and the consumer
MUST agree on the physical base, the consumer's components MUST be a subset of
the producer's, and the two masks MUST have the same lowest channel bit.

A consumer input the producer does not write is not a link failure in general:
the record carries only the slots the producer wrote, so the read lands on
whatever the ring holds there. Two inputs are exempt because the rasterizer
generates them, the point-sprite coordinate and a coordinate-replaced texture
set. The driver-inserted point-passthrough geometry stage is linked under a
stricter rule that requires every input, with its colour input exempt when the
vertex stage writes none.

Two failures are refused rather than corrected: a record whose kernel entry or
checked root falls below the GPU virtual-address base, because the pipe would
fetch or dereference an address the client map never covers; and a link that
breaks one of the rules above.

## Open questions

- Whether the VS slot ever launches. The copy exists so a fetch from that slot
  finds code; a launch from it would run the vertex program a second time, and
  no shape here establishes that it does not.
- What the two fusion bits fuse. Neither is ever set, so no launch shape shows
  what a set bit does.
- Whether a slot's type must match the role the slot runs. The ABI puts a type 3
  record in a slot enabled as a domain stage, so the two need not agree in
  general; what the hardware does with a record whose type names another role is
  UNRESOLVED.
- Whether the streamout slot can carry a role other than its copy kernel.
