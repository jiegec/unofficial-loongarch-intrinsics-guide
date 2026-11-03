# Performance

This appendix gives relative cost measurements for shader instructions. The
numbers are wall-clock nanoseconds with the submission overhead removed. They are
not architectural cycles, and their precision is limited; they are useful for
ranking instructions and for spotting large differences, not for cycle-accurate
modelling.

## Method

A benchmark program contains K copies of the target instruction. Two patterns are
measured:

| Pattern | Kind | Construction |
|---------|------|--------------|
| Dependency chain | Latency | Each copy consumes the previous destination. |
| Independent streams | Throughput | Eight independent register streams, no cross-copy dependency. |

The per-operation cost is the slope of wall-clock time against K. The fixed
submission and readback overhead cancels in the slope. The fit quality (R²) is
reported with each slope.

## Reading the columns

| Column | Meaning |
|--------|---------|
| Latency | Dependency-chain cost, relative wall-clock nanoseconds. |
| Throughput | Independent-stream cost, relative wall-clock nanoseconds. |
| Ratio | Throughput divided by latency; a value above one means the unit absorbs independent work. |
| R² | Fit quality. A value below 0.5 is not a trustworthy slope. |
| Confident | Both slopes are positive and well fitted. |

## Table

{{ lg200_latency_table() }}

## Accuracy

The shader ISA has no shader-readable clock. The time counter is a device clock
and must be calibrated against a host clock; the benchmark uses host wall-clock
time around a submission. The measurement floor is the submission and readback
overhead, which is much larger than one instruction. A single-point timing is
therefore meaningless; only the slope across copy counts is used.

The measurement is sensitive to the shader's register staging, to the shader
cache, and to the device's clock state. A number is comparable within one
measurement session, not across devices or clock settings.

> UNRESOLVED: the architectural cycle cost of every instruction, and the
> relationship between the relative numbers and the issue width of the vector
> unit.

## Improving the numbers

A more precise measurement needs one of:

- a shader-readable clock;
- a hardware performance counter that counts issued instructions per unit;
- a calibrated device clock with a known relationship to the shader clock.

Until one of these is available, the table is a relative ranking with the
accuracy stated above.
