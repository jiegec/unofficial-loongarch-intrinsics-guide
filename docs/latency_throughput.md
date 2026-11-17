# Latency and Throughput of Instructions

Latency and throughput (IPC) of each instruction. Values that depend on the
operands are given as a range. The LA464 (3C5000) and LA664 (3C6000)
measurements come from [LoongPerf](https://jia.je/LoongPerf/), an open
database of instruction-level performance data measured on real LoongArch
hardware; the other machines were measured with `code/measure.cpp` in this
repository.

{{ latency_throughput_table() }}
