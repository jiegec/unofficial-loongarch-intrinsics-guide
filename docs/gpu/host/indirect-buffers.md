# Indirect Buffers

A graphics submission is an indirect buffer: a GPU virtual address and a length
in bytes. The buffer contains an envelope, the state record and, for a batched
submission, several state records. This chapter defines the container. The
record grammar is in [State records](../graphics/state-records.md) and the state
fields are in [State object](../graphics/state-object.md).

![Command nesting](../assets/fig-command.svg)

The figure shows the containment of a submission, a ring packet, an indirect
buffer and a state record; the draw is the record, not a separate packet.

## Full-state envelope

The common form is 528 dwords:

| Dword | Content |
|------:|---------|
| 0 | `0x0000_0f04`: state-record opcode. |
| 1 | `10`: full-state mode. |
| 2 | `0x2000_0001`: window descriptor, start 0 and length 512. |
| 3–514 | The 512-dword state object. |
| 515 | `0x0000_0003`: state terminator. |
| 516 | `0x0000_0f05`: end command. |
| 517–527 | Zero padding. |

The three header dwords are the state-record header; dword 2 is a window
descriptor, not an opaque value. The state object starts at dword 3, so a state
offset is relative to that dword, and the packet byte offset is the state offset
plus 12.

There is no separate draw packet. The draw's own fields are the **Draw Info**
record at state offset `0x000`: 36 bytes, the first nine state dwords. The rest
of the 512-dword state object describes the shaders, render targets, viewports
and the other state the draw consumes. The record is the draw.

## Batching

Consecutive draws may share one indirect buffer. Only the **first** record of the
buffer is the full 516-dword form (528 dwords with its tail); later records are
delta records of 4 through 516 dwords, and the cursor advances by the record's
own length. The next header
overwrites the previous record's end command:

```text
record 0 (full):  header, 512 state dwords, 0x3, 0xf05, pads
record 1 (delta): 0xf04, windows, 0x3, 0xf05, pads
record 2 (delta): 0xf04, windows, 0x3, 0xf05, pads
```

The submitted length MUST restore the last record's end command and its eleven
pad dwords: twelve dwords beyond the last record. A length without that tail
leaves the graphics ring waiting for an end that never arrives.

The current driver sizes a batch for 80 draws, 64 segments and 64 indirect
buffers; these are host choices, not architectural limits.

## Border-table preamble

When a draw uses border colours, a six-dword preamble precedes the first state
record:

| Dword | Content |
|------:|---------|
| 0 | `0x0000_00a1` |
| 1 | Border-table address, low 32 bits: `0x001a_8000` |
| 2 | Border-table address, high 32 bits: `0x0000_0008` |
| 3 | `0x0000_0f05` |
| 4 | Zero |
| 5 | Zero |

The preamble MUST be the first block of the buffer. With a preamble present, the
first state header starts at dword 6 and the terminator and end command shift
accordingly. The preamble is emitted once per command context.

## Packets outside a state record

Three packet families sit between state records. They do not fit the fixed
528-dword block and cannot be batched with a draw.

| Purpose | Packet | Placement |
|---------|--------|-----------|
| Query begin | `(selector << 8) OR 0x0d` | Front of the buffer. |
| Query end | `0x3`, `(selector << 8) OR 0x0c`, address low, address high | After the state terminator. |
| Timestamp begin | `(1 << 8) OR 0x13`, address low, address high | Front of the buffer. |
| Timestamp end | `0x3`, `(0x200 << 8) OR 0x13`, address low, address high | Before the end command; the second sample is eight bytes past the first. |
| Streamout end | `0x0200_000c`, address low, address high, `0x16` | Between the state terminator and the end command. |

Query selectors are occlusion `0x400000`, primitives generated `0x2000` and
primitives emitted `0x20000`. A timestamp packet MUST be closed by a terminator
and an end command; otherwise the completion fence never signals.

## Alignment and limits

An indirect buffer MUST be mapped executable. Its length MUST be a multiple of
four bytes and no larger than `0x00ff_ffff` dwords. The kernel's admission check
establishes 4-byte alignment only; a 32-byte start for GPIPE/BPIPE or a 64-byte
start for CPIPE is a host convention, and whether the command processors require
it is UNRESOLVED.

## Open questions

- The meaning of the full-state mode word beyond the value 10.
- Whether a state record may start at a non-zero window for the first draw of a
  context.
- The exact packet opcodes for the query and timestamp forms outside the values
  listed here.
