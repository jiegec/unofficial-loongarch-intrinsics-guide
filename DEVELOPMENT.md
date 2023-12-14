# Notes for Developer

- `main.py`: used for mkdocs-macros-plugin, implement functions used in markdown.
- `check.py`: check generated docs against gcc intrinsics document.
- `check_instr.py`: check generated docs against binutils opcodes.
- `code/find_vd_source.py`: locate functions that have vd/xd as a source operand.
- `code/gen_impl.py`: generate C implementations for intrinsics.
- `code/gen_tb.py`: generate testbench for intrinsics.
- `code/untest.py`: list untested intrinsics.
- `code/gen_measure.py`: generate `measure.h` to measure latency and throughput of instructions.
- `code/Makefile`: use `make run` to validate C implementations against hardware, use `make run-measure` to measure latency and throughput.