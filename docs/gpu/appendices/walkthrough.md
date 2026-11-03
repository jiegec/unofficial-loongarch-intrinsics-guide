# Walkthrough: One Draw

This chapter follows one minimal draw from the user-space interface to the
shader. It introduces no new facts; every step links to the chapter that defines
it.

## 1. Create the context

The client opens the render node and creates a context with `CTX`. The context
carries the priority and the reset state. A graphics submission later names this
context.

See [User-space interface](../host/uapi.md#device-and-context).

## 2. Allocate buffers

The draw needs:

| Buffer | Use |
|--------|-----|
| Indirect buffer | The state record and the command stream. |
| Vertex buffer | Attribute data. |
| Colour buffer | The render target. |
| Fence buffer | Completion sequence number. |

Each buffer is created with `GEM_CREATE` and mapped with `GEM_VA`. The indirect
buffer is mapped readable, writable and executable; the vertex buffer is
readable; the colour buffer is writable.

See [Buffer objects](../host/buffer-objects.md).

## 3. Build the state record

The indirect buffer starts with a state record:

```text
0x00000f04
10
0x20000001
<512 state dwords>
0x00000003
0x00000f05
<pad>
```

The state object contains:

| Block | Content |
|-------|---------|
| Draw Info | Primitive type, count, instance count, vertex offset, index buffer. |
| Pipe Enables | Culling, fill mode, MSAA, viewport index. |
| Viewport | Scale and offset, scissor, depth range. |
| Depth Control, stencil, blend | Output-merger state. |
| Colour buffer | Format, type, tiling, dimensions, pitch. |
| Shader Config | Domain enable, domain-as-vertex. |
| Shader Info slot 2 | Vertex record: sizes, entry, roots. |
| Shader Info slot 5 | Fragment record: sizes, entry, roots. |

See [State records](../graphics/state-records.md) and
[State object](../graphics/state-object.md).

## 4. Build the command stream

The record is followed by the BPIPE commands that draw:

```text
CTXR(0x0a)
CFGW(0x01, payload_dwords)
<payload>
DRAW(0x03)
SYNC(0x05, stage 0, selector 0xf)
```

The payload selects the primitive box and, for a blit, the source texture. For a
plain draw, the state record is the draw and BPIPE carries the draw command.

See [BPIPE](../host/bpipe.md).

## 5. Submit

The submission is a `CS` ioctl with:

| Chunk | Content |
|-------|---------|
| `IB` | Address, length, engine, ring. |
| `FENCE` | Fence buffer handle and offset. |
| `BO_HANDLES` | The buffers the submission reads or writes. |

The kernel checks that the IB is executable and that every buffer is resident,
places the IB on the graphics ring, and returns a sequence number.

See [Submissions](../host/submissions.md).

## 6. Execute

GPIPE consumes the ring and executes the IB. It applies the state record and
launches the vertex stage. The vertex stage reads attributes through `s8:s9` and
writes its output to SIO0. The rasterizer assembles primitives and generates
fragments. The fragment stage reads its inputs and writes colour to SIO1. The
output merger applies depth, stencil and blend and writes the colour buffer.

See [Graphics pipeline](../graphics/pipeline.md),
[Vertex input](../graphics/vertex-input.md) and
[SIO](../shaders/io.md).

## 7. Wait

The client waits on the fence with `WAIT_CS` or `WAIT_FENCES`. A completed fence
means the submission can no longer access its buffers. The client may then read
the colour buffer through its CPU mapping, or use it as a texture in a later
submission.

See [Submissions](../host/submissions.md#completion).

## What can go wrong

| Symptom | Likely cause |
|---------|--------------|
| Ring stops consuming | IB length does not include the end command and pad dwords. |
| No fragments | Degenerate scissor, culling, or a viewport outside the guardband. |
| Wrong colours | Fragment output channel order or blend state. |
| GPU fault at address 0 | A zero descriptor base or a missing shader record. |
| Fence never signals | A timestamp or query packet is not closed by a terminator and end command. |

## Where to go next

- [Instruction reference](../isa/reference.md) for the rows in the shaders.
- [Descriptors](../shaders/descriptors.md) for the resource records.
- [Formats and layout](../graphics/formats.md) for the colour buffer.
