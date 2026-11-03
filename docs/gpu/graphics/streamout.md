# Streamout

Streamout writes generated vertices to buffers through descriptors. It is the
transform-feedback path. This chapter defines the descriptor records, the
streamout stage record and the end packet. The shader stages are in
[Stages](../shaders/stages.md).

## Transform feedback

The current implementation writes transform feedback directly from the vertex or
geometry stage into its target buffers. The stage's shader-info record points at
the destination descriptors, and the stage stores each record through a buffer
descriptor.

The streamout stage record is a separate kernel in slot 4 with shader type 6.
Its shape is:

| Property | Value |
|----------|-------|
| Shader type | 6 |
| VGPR size | 2 |
| VGPR initial count | 2 |
| Tail | `0x20` |
| Argument 2 | Record capacity |
| Root | Read-write buffer root only |

No established graphics path installs the streamout stage. Its hardware
contract is therefore UNRESOLVED; a host MUST NOT depend on it.

## Descriptor records

A transform-feedback target is a buffer descriptor whose base, stride and byte
size describe the destination. The record count and the record stride are
supplied by the stage's argument block.

The destination descriptor is a 16-byte buffer descriptor; its format is in
[Descriptors](../shaders/descriptors.md).

## End packet

A streamout end packet is placed between the state terminator and the end
command:

```text
0x0200000c
address_low
address_high
0x00000016
```

The packet closes the streamout stream and orders its writes. A packet placed
outside that range does not close the stream.

## Open questions

- The hardware streamout stage contract: how its read-write root is hydrated and
  how it loops over records.
- Whether the record capacity is a byte count or a record count.
- The interaction between streamout and a geometry stage that emits several
  primitives.
