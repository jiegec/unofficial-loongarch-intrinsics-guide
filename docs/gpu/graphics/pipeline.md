# Graphics Pipeline

A graphics submission turns a draw description into fragments and surface
writes. This chapter describes the order of the stages and the boundary between
fixed-function and programmable work. The state fields are in
[State object](state-object.md); the draw description is in [Draw](draw.md).

![Graphics pipeline and SIO](../assets/fig-pipeline.svg)

The figure shows the stage order, the SIO rings between the programmable stages
and the fixed-function blocks that consume the result.

## Stage order

A draw passes through the following stages:

1. **Vertex fetch.** Attribute descriptors supply the vertex inputs.
2. **Vertex stage.** A shader runs once per vertex and writes its output to
   stream SIO0.
3. **Geometry stage (optional).** A shader reads the SIO0 record, emits
   primitives and writes SIO1.
4. **Primitive assembly and rasterization.** The rasterizer assembles
   primitives, clips them, applies the viewport transform, culls faces and
   generates fragments.
5. **Fragment stage.** A shader runs once per fragment, reads interpolated
   inputs and writes colour, depth and coverage to SIO1.
6. **Output merger.** Depth and stencil tests, blending and the write mask
   produce the final surface values.

A streamout stage may run as a separate kernel. It is not part of the per-draw
pipeline in the current implementation; transform feedback is written by the
vertex or geometry stage.

## Fixed and programmable

| Stage | Kind | Selected by |
|-------|------|-------------|
| Vertex fetch | Fixed | Attribute descriptors, Draw Info |
| Vertex | Programmable | Shader-info slot 2 with the domain-as-vertex bit |
| Geometry | Programmable | Shader-info slot 3 |
| Rasterizer | Fixed | Pipe Enables, viewport and guardband |
| Fragment | Programmable | Shader-info slot 5 |
| Depth and stencil | Fixed | Depth Control, stencil face records |
| Blend | Fixed | Blend Control and per-target blend words |
| Texture | Fixed | Image and sampler descriptors |
| Streamout | Programmable | Shader-info slot 4 |

There is no hull stage and no tessellation. Slot 1 is not programmable. The
domain slot carries the ordinary vertex role when the domain-as-vertex bit is
set.

## Stage interfaces

The stages exchange data through two mechanisms:

- **SIO rings.** The vertex stage writes its output to SIO0; the geometry stage
  reads SIO0 and writes SIO1; the fragment stage writes SIO1. SIO is a
  channel-addressed interface, not memory. See [SIO](../shaders/io.md).
- **Descriptor-bound memory.** Uniform buffers, storage buffers, images and
  samplers are reached through descriptor tables whose roots are in the
  shader-info record. See [Descriptors](../shaders/descriptors.md).

## Shader selection

The shader config word enables the stage slots:

| Bit | Stage |
|----:|-------|
| 0 | Vertex slot |
| 1 | Hull slot |
| 2 | Domain slot |
| 3 | Geometry slot |
| 4 | Streamout slot |
| 5 | Domain as vertex |
| 6 | Domain includes geometry |
| 7 | Vertex includes hull |

A vertex-only draw enables the domain slot and the domain-as-vertex bit. A draw
with geometry enables the domain slot, the geometry slot and the domain-as-vertex
bit. The vertex slot is not used for execution in either case; it holds a
prefetch-safety copy of the vertex record.

## Fragment output

The fragment stage writes colour channels to SIO1. The first colour target
starts at channel `channels << 7`, where `channels` counts the channels of the
preceding targets. A target contributes two channels for a 16-bit format, four
for a 32-bit format and one for a packed integer format. A written depth value
occupies channel slot 0. The export count is the total colour channels plus one
when depth is written; a fragment stage that writes nothing still exports one
channel.

## Diagram

The stage flow, the SIO rings and the descriptor roots are shown in the pipeline
figure. The figure adds the data path that the table above cannot show: which
stage writes which ring and which fixed-function block consumes it.

## Open questions

- The exact hand-off between the rasterizer and the fragment stage for helper
  lanes.
- Whether a geometry stage can be fused with the vertex stage for every input
  shape.
- The behaviour of the pipeline when a stage is enabled but its record is all
  zero.
