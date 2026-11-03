# Performance

The table in this appendix carries no cycles: each value is a relative wall-clock
cost fitted from whole submissions. It compares instructions and shows which ones
gain from independent work; it cannot model a cycle budget.

## The two patterns

Each row is measured twice, from one canonical operand form, in two programs that
differ only in how the copies are wired.

| Column | Program shape | What it isolates |
|--------|---------------|------------------|
| `lat ns/op` | Chain: each copy reads the register the previous copy wrote. | The wait for a result the next instruction consumes. |
| `tp ns/op` | Streams: each copy's vector source advances two registers, wrapping after eight. | The cost of one more instruction while earlier ones are in flight. |

Both programs hold many copies of the instruction and one result store outside the
loop; all source values are zero.

A chain forms only when the instruction both writes and reads a vector register,
so that the next copy's source is the register the previous copy wrote. A
scalar-result row builds the same program both ways, so its two columns are repeat
runs of one program. A compare row writes a condition code that no later copy
reads, so neither of its programs has a chain. In both families `lat` is not a
latency. The stream program also writes one destination in every copy, so it is
eight-way in its sources only.

## Reading a cell

| Column | Rule |
|--------|------|
| `lat ns/op`, `tp ns/op` | Relative wall-clock nanoseconds, shown when the slope is positive and its fit is at least 0.5. |
| `tp/lat` | A number only when both cells are. Below one, independent work is absorbed; above one the stream program measured slower per operation, which is not evidence of an issue limit. |
| `lat R2`, `tp R2` | Quality of each least-squares fit. |
| Confident | Yes when both slopes are positive and both fits exceed 0.5. |

A `-` is neither zero nor cheap: the fit resolved no cost for that row, and a
negative slope is suppressed. A missing cell says nothing about the instruction.

## What the table does not establish

- **Cycles.** No instruction reads a counter. When a submission carries
  timestamps, the submission wrapper writes a pair of 64-bit stamps at argument
  offsets `0x3000` and `0x3008`; they bracket the whole submission, not one
  instruction. They count GPU clock ticks, so their rate depends on the clock
  state and has to be established for the part in use.
- **A per-instruction time.** Submitting a program and reading the result back
  costs far more than one instruction, so a single submission cannot resolve one;
  the fitted cost comes from how the total grows with the number of copies.
- **Operand data.** All sources are zero, so the table says nothing about
  data-dependent cost: a denormal, a NaN or a large exponent is not represented.
- **Fragments.** A 4 by 4 target runs the program once per fragment, so each copy
  runs sixteen times; the fitted cost is not divided by that count and is not a
  per-lane figure.
- **Comparability.** A value is comparable with other rows of this table; clock
  state and program shape move it.

> UNRESOLVED: the architectural cycle cost of every instruction; the relation
> between these numbers and the issue width or occupancy of the vector unit;
> whether the sixteen fragments occupy one wave.

## Using the numbers

- Price a producer-consumer pair with `lat`; price an interleaved loop with `tp`
  times the operations in flight.
- Do not sum `lat` along a pipelined loop, and do not use it for a scalar or
  compare row: those rows have no chain.
- A `-` row is unknown cost, not free.
- Only the canonical operand form has an entry; an immediate variant is a
  different row and inherits nothing.
- Memory, sampler and control-flow rows have no entry: only register-operand forms
  are built.

## Table

{{ lg200_latency_table() }}
