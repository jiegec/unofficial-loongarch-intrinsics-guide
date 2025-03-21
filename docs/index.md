# Unofficial LoongArch Intrinsics Guide

[![DOI](https://zenodo.org/badge/730251036.svg)](https://doi.org/10.5281/zenodo.14169508)

This is the Unofficial LoongArch Intrinsics Guide by Jiajie Chen et, al. The documentation is arranged from the following sources:

- QEMU
- GCC
- Observations from real hardware incl. 3C5000 and 3A6000

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

Contributions are welcome.
