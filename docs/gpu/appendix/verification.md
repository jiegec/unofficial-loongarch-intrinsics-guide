# Verification Tooling

This section ships with the machinery used to establish the behaviour documented
in these pages: a CPU oracle with deterministic case vectors, a hardware runner
that executes the same vectors on a chip, and a differential microbenchmark.

## Case vectors

Every machine row in the instruction table carries deterministic case vectors.
A vector names its inputs and its expected result, so the same vector can be run
twice — once against a CPU model of the semantics and once on hardware — and the
two results compared bit for bit.

The vectors are the reason the instruction pages can state boundary behaviour:
a row's description says what has been established for that row's exact variant,
and anything not covered by a vector is marked `UNRESOLVED` rather than
described.

## CPU oracle

```sh
make -C code/lg200 -j verify
```

This compiles one oracle per mnemonic together with the shared oracle library
and runs every vector, reporting a count of instructions and vectors.  It needs
no device and no driver.

Two conventions matter when reading an oracle:

* `<mnemonic>.h` is the executable reference pseudocode for the row.  It is
  included by the oracle and is written in the vocabulary of the oracle library
  (bit widths, floating-point policy, saturation helpers).
* `<mnemonic>.cpp` holds the oracle body and the case vectors.  Floating-point
  vectors carry an explicit policy (rounding and denormal handling) so that a
  vector states which mode it exercises.

Additional boundary vectors can be generated:

```sh
make -C code/lg200 gen-cases      # regenerate the boundary rows
```

Editing a single description cell is done through the table editor, which reads
and writes the whole table and leaves a snapshot:

```sh
python3 code/lg200/set_desc.py <mnemonic> <markdown-file>
python3 code/lg200/set_desc.py --show <mnemonic>
python3 code/lg200/set_desc.py --check
```

## Hardware runner

```sh
# The deb-staged libdrm is older and lacks loonggpu_userq_free, so a plain make
# fails at link and removes the binary.  Point at the current one:
PKG_CONFIG_PATH=/home/lin/work/build/libdrm-loonggpu-prefix/lib/pkgconfig \
  make -C code/lg200/gpu build/lgpu-gpu   # build the runner
python3 code/lg200/gpu/run_gpu.py --mnemonic v_add_u32
python3 code/lg200/gpu/run_gpu.py       # the whole universe
python3 code/lg200/gpu/run_gpu.py --dry # assemble only, no submission
```

The runner assembles a row's vectors into a fragment program, submits once on
the graphics ring, reads the result buffer back and compares the words with the
expected values.  Each instruction runs in its own process with a fence timeout,
so a hang is contained to that case.

Rows whose behaviour depends on launch state the runner cannot stage — flat
memory, formatted buffer loads, matrix reads, texture fetch and sampling, and a
handful of others — execute in **observed** mode: the row records that it ran and
what was observed, instead of comparing against an independent oracle.  A page
that documents such a row states the observation and marks anything beyond it
`UNRESOLVED`.

Results land in the runner's build directory, one file for a full matrix run and
one for a single-instruction run, with a per-instruction log.

`run_gpu.py golden [--check]` regenerates or diffs the assembled listings of one
representative program per carrier shape; a change in the listing is then a
conscious update rather than an accident.

## Microbenchmark

```sh
python3 code/lg200/run_bench.py
python3 code/lg200/run_bench.py --only v_add_u32,s_add_i32
```

The benchmark builds one program containing K copies of a target instruction and
measures the wall-clock cost of one submission per K.  Two patterns are
measured: a dependency chain (latency) and independent register streams
(throughput).  The per-operation cost is the slope across copy counts, so the
fixed submission and readback overhead cancels.

Numbers are relative wall-clock nanoseconds, not architectural cycles: the
shader ISA exposes no shader-readable clock.  A slope that is not positive or
whose fit is poor is reported as unavailable rather than as a value, and the
rendered tables show such rows as "-".

## What the tooling does not cover

* Launch-state behaviour that the runner cannot stage; those rows are observed,
  not compared.
* Behaviour outside a row's exact variant: a vector pins one operand form, and a
  different form of the same mnemonic is a different question.
* Performance in architectural units; see the caveat above.
* Anything that requires a second command stream, a different pipe, or state
  the fragment-stage runner does not build.

## Compute (KCD) carrier

The fragment-stage carrier cannot stage every launch state.  The shared-memory
matrix forms need a workgroup allocation, and the runner classifies them -
`load_sm_matrix_*` and every `v_mma_*` - as observation-only for that reason.
The runner exposes the graphics submissions (`submit_ps`, `submit_gs`) and the
two probes; measuring those rows needs a compute submission path, so the plane
and lane layout they still leave open stays open here.

Results that have been measured through another compute path are recorded with
the rows themselves, in
[Vector ALU](../isa/family-vector.md#matrix-multiply-accumulate) and
[Memory and Image](../isa/family-memory.md#shared-memory).
