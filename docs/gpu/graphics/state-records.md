# State Records

A state record is the command sequence that tells the command processor what the
state object contains. It is not the state object itself: a record is a header, a
set of write windows, and a terminator. This chapter defines the record grammar.
The state fields are in [State object](state-object.md); the indirect buffer that
contains the record is in [Indirect buffers](../host/indirect-buffers.md).

![State record forms](../assets/fig-state-record.svg)

The figure contrasts the full form with a delta form and shows where a batch
record ends.

## Record grammar

A record has one of two forms.

**Full form.** The record writes all 512 state dwords:

```text
0x0000_0f04        record opcode
10                 full-state mode
0x2000_0001        range: window opcode 1, start 0, length 512
<512 dwords>       the state object
0x0000_0003        terminator
```

**Delta form.** The record writes one or more windows:

```text
0x0000_0f04        record opcode
<descriptor>       window descriptor
<data>             one dword per state dword in the window
...                more windows
0x0000_0003        terminator
```

A window descriptor is a 32-bit word:

| Bits | Field | Meaning |
|------|-------|---------|
| `[7:0]` | Opcode | 1 for a state window. |
| `[19:8]` | Start | First state dword covered by the window. |
| `[31:20]` | Length | Number of state dwords in the window. |

The length MUST be nonzero and the window MUST stay inside the 512-dword block.
A record whose delta is no smaller than the full form is emitted as the full
form.

## Semantics

The command processor keeps the state block between records. A delta record
changes only the dwords named by its windows; every other dword keeps the value
from the previous record. A full record replaces the whole block.

A record is applied at the draw that carries it. The state object is not a
separate command: a record that passes the command processor is the draw's
state.

## Terminator and end command

Every record ends with the terminator `0x0000_0003`. A full-state envelope then
carries the end command `0x0000_0f05`, followed by eleven zero dwords.

In a batched buffer, the next record's header overwrites the previous record's
end command; only the last record keeps its end command. The submitted length
MUST include the last record's end command and its eleven pad dwords.

## Batching

The full form occupies 528 dwords, of which 517 are used. The batch cursor
advances by 516 dwords, so the next record starts where the previous record's
end command was. A delta record is variable-length and starts at the cursor left
by the previous record.

## Why delta records exist

A draw usually changes a small part of the state: a few addresses, a viewport,
a shader record. Writing 512 dwords for every draw costs bandwidth and command
processor time. A delta record writes only the changed runs. The hardware
accepts both forms; a driver chooses.

## Validity rules

- The record MUST start with `0x0000_0f04`.
- A full record MUST carry the mode word `10` and the range word
  `0x2000_0001` before the 512 state dwords.
- A delta record MUST NOT carry the mode word.
- Every window MUST have a nonzero length and a start plus length no greater
  than 512.
- The record MUST end with the terminator.
- A full-state envelope MUST end with the end command and its pad dwords.

## Open questions

- The meaning of the mode word beyond the value 10.
- Whether a record may use the full form with a window other than start 0 and
  length 512.
- Whether a delta record may be the first record of a context.
