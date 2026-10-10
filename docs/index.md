# Unofficial LoongArch Intrinsics Guide

[![DOI](https://zenodo.org/badge/730251036.svg)](https://doi.org/10.5281/zenodo.14169508)

This is the Unofficial LoongArch Intrinsics Guide by Jiajie Chen et al. The documentation is arranged from the following sources:

- QEMU
- GCC
- Observations from real hardware incl. 3C5000(LA464), 3A6000(LA664), 3C6000(LA664), 2K1000LA(LA264) and 2K3000(LA364E)

Beyond SIMD intrinsics, this guide also documents the **LBT** (LoongArch Binary Translation) instruction set, used for emulating x86, ARM, and MIPS ISAs. See the [LBT Basics](lbt_basics.md) page for an overview.

The guide additionally covers the **Loongson GPU (LG200)** shader ISA and its
graphical ABI in the [Loongson GPU (LG200)](gpu/overview.md) section: the
768-instruction machine rows with encoding visuals and reference pseudocode,
the host interface and graphics state, and the shader ABI. Every instruction
row is pinned by deterministic case vectors that run both on the CPU oracle
suite and on the real LG200 through the PS-generic carrier (768/768 PASS).

The guide provides pseudo code for the SIMD intrinsics. The code assumes that the elements of the LSX/LASX vector registers can be accessed via members of a `union`:

```cpp
union lsx_register {
  uint8_t byte[16];
  uint16_t half[8];
  uint32_t word[4];
  uint64_t dword[2];
  uint128_t qword[1];
  float fp32[4];
  double fp64[2];
};

union lasx_register {
  uint8_t byte[32];
  uint16_t half[16];
  uint32_t word[8];
  uint64_t dword[4];
  uint128_t qword[2];
  float fp32[8];
  double fp64[4];
};
```

In hardware, the lower bits of FP/LSX/LASX registers are shared, that is:

- FP register f0 and LSX register v0 and LASX register x0 share the lowest 64 bits
- LSX register v0 and LASX register x0 share the lowest 128 bits

The GitHub repo is [jiegec/unofficial-loongarch-intrinsics-guide](https://github.com/jiegec/unofficial-loongarch-intrinsics-guide).

If you want to visit this site in Markdown format (e.g. you are an LLM), please read [llms.txt](https://jia.je/unofficial-loongarch-intrinsics-guide/llms.txt).

Contributions are welcome.
