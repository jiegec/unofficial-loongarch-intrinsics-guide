# Indirect Buffers

A submission is a chunk array. Every indirect-buffer chunk names one GPU virtual
address and a byte length, and the bytes at that address are the command stream
this chapter defines. The ring packet that names a buffer carries the length in
a 24-bit dword field, so the field, not the memory, sets the largest legal
buffer. The record grammar is in
[State records](../graphics/state-records.md), the state fields in
[State object](../graphics/state-object.md), the chunk array in
[Submissions](submissions.md).

![Command nesting](../assets/fig-command.svg)

The figure shows one submission, the packet that names a buffer, and the parts
of the buffer: the border preamble, a full record, a delta record and the tail.
The draw is the record; there is no separate draw packet.

## The envelope

A buffer that carries one full state record and nothing else is 528 dwords.

| Dwords | Content |
|-------:|---------|
| 0 | `0x0000_0f04` — record opcode. |
| 1 | `10` — full-state mode. |
| 2 | `0x2000_0001` — window descriptor: opcode 1, start 0, length 512. |
| 3–514 | the 512-dword state object. |
| 515 | `0x0000_0003` — record terminator. |
| 516 | `0x0000_0f05` — end command. |
| 517–527 | eleven zero dwords. |

Dwords 0–2 are the state-record header. Dword 2 is a window descriptor with the
same encoding a delta window uses, so the full form is one window over the whole
block: bits `[7:0]` opcode 1, `[19:8]` start 0, `[31:20]` length 512. Dwords
0–515 are the record, 516 dwords. Dwords 516–527 are the tail: the end command
plus eleven zero dwords.

A state offset is a dword index into the block that starts at dword 3, so state
dword *n* sits at byte `12 + 4n`. The draw's own fields are the 36-byte Draw
Info record at state offset `0x000`, the first nine state dwords; the other 503
describe the shaders and resources the draw consumes.

## Records and the cursor

The end command closes a command block, and the last one in a buffer is what the
submission retires on. Eleven zero dwords follow it, and zero is the No
Operation command, so running them executes nothing.

A batched buffer holds several records back to back and one tail after the last
of them. The cursor is the dword index where the next record starts. A full
record ends on its terminator, the end command sits in the slot after it, and
the next record's `0xf04` overwrites that slot — so a full record advances the
cursor by 516 and not by 528. A delta record ends on its own terminator, and the
cursor moves one past its last dword, so it advances by its own length.

The record that opens a buffer is full: at a buffer boundary the command
processor is not known to hold the state block, so the opening record
re-establishes all 512 dwords. A later record states only what changed, as one
or more windows. A window names a run of the block: bits `[7:0]` opcode 1,
`[19:8]` first state dword, `[31:20]` length. It MUST have a nonzero length and
MUST stay inside the 512-dword block, and a delta MUST NOT grow past 516 dwords;
a driver that would exceed that sends the full form instead.

A full record MAY appear anywhere in a buffer, not only first. A driver sends
one when its write log overflows, when a changed dword is not in the log, or
when nothing changed — a record of `0xf04` `0x03` with no window is not a form
the command processor is known to accept. The full form is always correct; the
delta form is an optimisation.

The submitted length MUST cover the last record's end command and the eleven
zero dwords behind it; only a packet that is a complete block of its own may
take that slot. A length that stops at the last terminator leaves no end command
in the buffer, and the submission never retires.

## Border-table preamble

A sampler that clamps to a border colour reads a 64 KiB table at the fixed
address `0x0000_0008_001a_8000`. A six-dword preamble registers it and leads the
buffer:

| Dword | Content |
|------:|---------|
| 0 | `0x0000_00a1` — preamble opcode. |
| 1 | table address, bits 31:0 — `0x001a_8000`. |
| 2 | table address, bits 63:32 — `0x0000_0008`. |
| 3 | `0x0000_0f05` — end command. |
| 4 | zero |
| 5 | zero |

The preamble is a complete block: its own end command closes it and the record
that follows is a new block. It does two jobs — it names the border table, and
it is what makes the command processor re-read the descriptor roots. A changed
texture binding between two records of one buffer is otherwise not re-read.

The preamble MUST be the first block of a buffer and MUST NOT occur twice in
one. A preamble in the middle makes the processor re-parse the stream as if a
buffer started there, and it then walks into an unmapped address. A draw that
needs the descriptor roots re-read therefore opens a new buffer instead of
inserting a preamble mid-buffer, and the first buffer of a command context
carries one, which is what registers the table.

## Packets outside a state record

Five packet forms sit beside a state record. None of them shares a buffer with a
second draw: a buffer that carries one is submitted on its own.

| Purpose | Dwords | Packet |
|---------|-------:|--------|
| Query begin | 1 | `(selector << 8) OR 0x0d` |
| Query end | 4 | `0x3`, `(selector << 8) OR 0x0c`, address low, address high |
| Timestamp begin | 3 | `(1 << 8) OR 0x13`, address low, address high |
| Timestamp end | 4 | `0x3`, `(0x200 << 8) OR 0x13`, address low, address high |
| Streamout end | 4 | `0x0200_000c`, address low, address high, `0x16` |

The low byte of a packet's leading word is an opcode: `0x0c` samples a counter,
`0x0d` opens one, `0x13` is a timestamp, and `0x3` is the Submit word, the
value that also terminates a state record. The streamout-end tuple closes with
`0x16`, a memory barrier. The leading `0x3` of an end packet is that Submit
word; the three dwords behind it are the counter or timestamp packet, so a
packet placed directly behind a state terminator reuses the terminator as its
Submit word.
`0x0200_000c` is the query-end word carrying the primitives-emitted selector
`0x20000` in bits `[31:8]`. A counter word carries one of four selectors:
occlusion `0x400000`, primitives generated `0x2000`, primitives emitted
`0x20000`, and transform-feedback primitives written `0x200`. A timestamp
packet carries a phase in the same bit position instead — 1 for the first sample
and `0x200` for the second, which is written eight bytes past the first. That
`0x200` is a phase, not the `0x200` selector; the packet opcode tells the two
apart. What each counter counts is in [Queries](../graphics/queries.md).

A begin packet precedes the record. An end packet follows the record's
terminator. The streamout-end tuple is a complete block that takes the
end-command slot: its four dwords replace `0xf05` and its `0x16` barrier
closes it, so no separate end command follows. A buffer that carries the
border-table preamble starts with it, and every other packet follows the
preamble.

UNRESOLVED: how an end packet is framed against the end command. One layout
keeps the end command in place and writes its own `0x3` and the packet into the
pad dwords behind it; another puts the packet directly behind the state
terminator and re-appends the end command last; a third keeps the end command
and puts two zero dwords between it and the packet. A driver SHOULD put the
packet directly behind the record terminator and re-append the end command
last: the packet then needs no Submit word of its own, and the buffer still
ends on the end command.

## Alignment and limits

A buffer MUST be backed by a present mapping in the submitting VM and MUST
appear in that submission's residency list; write permission is not required.
Its start address MUST be 4-byte aligned, and its length MUST be a nonzero
multiple of 4 bytes. The bytes behind the last buffer of a mapping MUST be
mapped: the command processor reads past the declared end, so a host keeps a
guard page behind its command memory.

| Bound | Value | Source of the bound |
|-------|-------|---------------------|
| Buffer length | `0x00ff_ffff` dwords | ring packet field, 24 bits |
| State block | 512 dwords | state object |
| Full record | 516 dwords | record |
| Tail | 12 dwords | record |
| Delta record | 4 … 516 dwords | record |
| Window | 1 … 512 dwords | descriptor fields, block size |

The number of draws in a batch and the number of chained buffers in one
submission are host choices, not device limits. A host MAY cap a batch at 80
draws and a submission at 64 chained buffers, and MAY place each chained buffer
on a 16 KiB boundary. A submission cannot chain more buffers than the ring has
room for its packets, which is the tighter bound.

## Open questions

- The meaning of the full-state mode word beyond the value `10`.
- Whether the command processor keeps the state block across a buffer boundary.
  A host that does not know sends a full record as the opening record of every
  buffer; a delta there would render with stale state if the block is dropped,
  and it would do so silently.
- What the terminator's variant flags, bits `[31:8]`, select. Every terminator
  here is `0x3` with those bits zero.
- Whether a buffer start needs more than 4-byte alignment. Host clients start
  buffers on much larger boundaries, but no command field or admission rule
  states a larger requirement.
