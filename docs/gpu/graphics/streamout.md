# Streamout

Streamout, or transform feedback, writes the vertices a draw has already
transformed into buffers. This chapter defines the target descriptor, the record
a capture writes, how many records a draw produces, the emit and cut rules behind
that count, and the end packet that samples the streamout counter. Stage selection
and the slot-4 record shape are in [Pipeline](pipeline.md#streamout).

## Capture paths

The direct path has the vertex or geometry stage store each captured component
into the target. The copy path has a shader-type-6 program in slot 4 copy 16-byte
records out of a payload region of the argument block; it needs slot 4 programmed,
SS Enable set, a written payload region, and a record count and stride in its
control pair. Its loop performs a scalar load on every iteration, a shape that
stalls the ring, so no draw enables it. A host MUST use the direct path; whether
the copy path completes at all is UNRESOLVED.

## Target descriptor

A target is a 16-byte buffer descriptor.

| Bits | Field | Streamout value |
|------|-------|-----------------|
| [47:0] | Base address | Target resource base plus the current write offset. |
| [61:48] | Stride | Zero. The record stride is not carried here. |
| [95:64] | Byte size | Write allowance in bytes, counted from the base. |
| [107:96] | Destination selects | X `[98:96]`, Y `[101:99]`, Z `[104:102]`, W `[107:105]`. |
| [110:108] | Number format | Float. |
| [114:111] | Data format | R32. |

Every other field is zero. The write offset folds into the base because there is
no per-draw offset word, and the byte size gates the stores: an address inside the
range lands, one past it drops. A host MUST clamp base plus size to the target's
mapping.

The direct path packs four descriptors, 16 bytes apart, at the vertex constant
root + `0x120`; the copy path packs one at read-write root + `0x20` (slot 2), the
only root the copy program reads, through `s0:s1`. A slot without a target MUST NOT
name address zero: a store there does not fault but aliases whatever the kernel
mapped, so point it at a scratch buffer, where a store the record-count guard
does not drop still lands in known memory. One copy descriptor means buffer 0
only; whether the copy path can capture several buffers is UNRESOLVED.

## Records and the record count

A record is one captured vertex. The direct path emits one 32-bit store per
captured component at the offset the capture layout assigns, and addresses record
`i` at `(vertex id - record base) * stride` with that buffer's stride; the record
base is the draw's first vertex id, so a non-indexed draw that starts above zero
still begins at record 0 of the current buffer offset. The copy path instead
walks `i * stride` from zero up to its record count and moves a 16-byte record
whole.

| Primitive mode | Records |
|----------------|---------|
| Points | One per vertex |
| Lines | `n - n mod 2` |
| Triangles | `n - n mod 3` |
| Line strip | `2 * (n - 1)` for two or more vertices, else none |
| Line loop | `2 * n` for two or more vertices, else none |
| Triangle strip, triangle fan | `3 * (n - 2)` for three or more vertices, else none |

Strips, fans and loops are expanded to a list before the capture, and quad modes
to a triangle list, so the count is the vertex count of the expanded list. A
trailing incomplete primitive produces no record, and a host MUST NOT write those
vertices: the buffer holds complete primitives plus a guard band, and the extra
record overruns it.

The count reaches the shader through the argument block, not the descriptor: the
record count is the dword at `0x1ce0` and the record base the dword at `0x1cf8`
(the geometry stage reads its own copy at its constant root); the copy path reads
its count and stride from the pair at `0x2f00` with the payload at `0x2e00`,
inside one 4-KiB page. Every capturing draw MUST program both: the shader
compares the record id with the count, computes offset 0 for a lane at or past it
and masks that store, so an unprogrammed count turns a padded wave into stores
over record 0.

## Emit and cut

A geometry stage emits vertices with `v_emit2` for points and lines and `v_emit3`
for triangles, and cuts the primitive stream separately. Each emit produces one
record, keyed on the runtime emit ordinal; a cut produces none, because it
restarts primitive assembly and leaves the record ordinal unchanged. Skipped
emits write nothing. The layout and the cut protocol are in
[Geometry](../shaders/geometry.md#emit-and-cut).

The record budget of a geometry invocation is its static emit-site count, or the
declared maximum vertex count when that count is not static; a stage whose
vertices are assembled into output primitives uses the assembled span. One output
primitive holds one record for points, two for lines, three for triangles.

## End packet

The end packet samples the streamout counter into a 16-byte, GPU-writable result
slot.

| Dword | Value |
|------:|-------|
| 0 | `0x0200_000c`: opcode `0x0c` in bits 7:0, selector `0x20000` in bits 31:8 |
| 1 | Result address, low |
| 2 | Result address, high |
| 3 | `0x16`, a memory barrier |

A draw on which a primitives-emitted query ends carries the packet, and that draw
MUST be submitted in a buffer of its own. A buffer that carries only packets uses
the three-dword form, dwords 0 to 2, and closes with the `0xf05` end command
instead of the barrier.

The packet writes an 8-byte value at its address. A host MUST NOT read it as the
API's primitives-written count, which is accumulated host-side because the direct
path does not feed the hardware streamout counter. What one unit of that counter
measures is UNRESOLVED.

The packet follows the state terminator. In a draw's record its four dwords start
at the dword that would otherwise hold the record's end command, and the pad
follows. Whether `0x16` also ends the buffer, or the end command must be
re-appended after the packet, is UNRESOLVED; a host SHOULD keep the packet outside
every state record.

## Supported modes

Four capture buffers, a 16-bit record stride in bytes, and 32-bit captured
components: a 16-bit capture output produces no store. A target is R32 float with
destination selects X, Y, Z and W. The modes listed above have a record count;
every other mode counts one record per vertex. Whether adjacency forms and patch
lists capture correctly is UNRESOLVED.

## Open questions

- What one unit of the streamout counter measures, and whether the copy path
  feeds it.
- Whether the end command must follow the end packet.
- What the slot-4 argument 2 record capacity counts.
- Whether the copy path can capture more than one buffer.
