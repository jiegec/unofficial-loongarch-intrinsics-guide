# Queries

The GPU counts occlusion, generated primitives and emitted primitives, and can
sample a timestamp. A query is a begin packet, the work, and an end packet that
writes a 64-bit result to memory. This chapter defines the selectors and the
packet placement.

## Selectors

| Selector | Counter |
|---------:|---------|
| `0x400000` | Occlusion; shared by the "any samples" and "samples" forms. |
| `0x2000` | Primitives generated. |
| `0x20000` | Primitives emitted. |

The generated and emitted counters are counted on the host rather than by the
GPU in the current path. The occlusion counter is a GPU counter.

## Begin and end

A query begins at the front of an indirect buffer and ends after the state
terminator:

```text
begin:  (selector << 8) OR 0x0d
end:    0x00000003
        (selector << 8) OR 0x0c
        result_address_low
        result_address_high
```

The result address is a GPU virtual address of a 64-bit word. The buffer that
receives the result MUST remain resident until the submission's fence signals.

## Query buffer

A query buffer is 0x4000 bytes. The result of one query is one 64-bit word; a
host that runs several queries in one buffer MUST give each its own word and
MUST NOT assume that the hardware writes a packed array.

## Timestamps

The Timestamp Query packet is the same begin/end shape with the timestamp selector:

```text
begin:  (1 << 8) OR 0x13, address_low, address_high
end:    0x00000003, (0x200 << 8) OR 0x13, address_low, address_high
```

The second sample is written eight bytes past the first. A timestamp packet MUST
be closed by a terminator and an end command; otherwise the completion fence
never signals.

## Placement

| Packet | Placement |
|--------|-----------|
| Query begin | Front of the buffer. |
| Query end | After the state terminator. |
| Timestamp begin | Front of the buffer. |
| Timestamp end | Before the end command. |
| Streamout end | Between the state terminator and the end command. |

A query packet cannot be batched with another draw. A draw that needs a
synchronised result is submitted standalone and waited on.

## Open questions

- The exact meaning of the occlusion "any" versus "samples" forms.
- Whether the primitive counters can be read without a host-side counter.
- The maximum number of queries in one buffer.
