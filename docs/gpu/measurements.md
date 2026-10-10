# LG200 Microbench Measurements

## How the numbers are produced

`code/lg200/run_bench.py` builds one PS program containing K copies of the
target instruction (K = 128…1536) and measures the wall-clock of one
submission per K.  The per-op cost is the **slope** of wall-clock vs K
(least squares over 5 copy points, 31 reps each, min-of-reps); the fixed
submission/readback overhead (~17 ms) cancels out.  Two patterns are
measured per instruction:

| Pattern | Kind | Template |
|---------|------|----------|
| Dependency chain | `lat` | each op consumes the previous destination of the chain, same lane |
| Independent streams | `tp` | 8 independent register streams, no cross-op dependency |

Every op executes on **all 16 fragment lanes** of the PS-generic carrier, so
both numbers are the wall-clock cost of one op per fragment-lane.

## Reading the columns

GPU measurement data is organized along two axes that the CPU (LSX/LASX)
tables don't need:

1. **Pattern**: latency (`lat`) vs throughput (`tp`).  On a CPU a single
   latency and a single IPC describe an instruction; on a GPU the two
   patterns can differ by an order of magnitude (issue-port structure,
   operand crossbar, SIMT lanes), and the ratio between them is itself the
   interesting quantity (how much ILP the unit can absorb).
2. **Confidence**: the fit R² of each slope plus an overall flag.  A slope
   ≤ 0 or R² < 0.5 means the per-op signal is below the ~17 ms submission
   floor; the value is not trustworthy and is rendered as "-".

| Column | Meaning |
|--------|---------|
| `lat ns/op` | dependency-chain cost, relative wall-clock ns |
| `tp ns/op` | independent-stream cost, relative wall-clock ns |
| `tp/lat` | ILP absorption ratio — how much faster the unit runs the same op without cross-op dependencies (>1 means the op is throughput-limited, not latency-limited) |
| `lat R²` / `tp R²` | fit quality of each slope (1.0 = perfect line) |
| Confident | both slopes > 0 with R² > 0.5 |

## Table

{{ lg200_latency_table() }}

## Caveats

- **Relative, not cycles**: the LG200 shader ISA exposes no shader-readable
  clock (getreg is PS-gated to 0; s_movindex_b32 is an SGPR-window read, not
  a timer; TIME_COUNT is host-side).  Numbers are wall-clock nanoseconds
  with the submission overhead removed.
- **Carrier-specific**: the PS-generic carrier stages operands through the
  fixed s4/v10 register windows; measured costs include that staging for
  memory/texture families (those are not in the table — only the
  register-pure targets are benchable this way).
- **Floor**: ~17 ms per submission vs ~5–200 ns per op means single-point
  timings are meaningless; only the slope across copy counts is used, and
  only when it is positive and well-fit.
