# LoongArch SIMD Basics

## Compiler Macros

GCC and Clang define feature macros for LoongArch SIMD when the corresponding target options are enabled:

- `__loongarch_sx`: LSX is enabled, for example with `-mlsx`.
- `__loongarch_asx`: LASX is enabled, for example with `-mlasx`.
- `__loongarch_simd_width`: the enabled SIMD vector width in bits. It is `128` for LSX and `256` for LASX.

GCC also defines `__loongarch_simd` when some LoongArch SIMD extension is enabled. Clang does not define this macro, so portable code should prefer `__loongarch_sx`, `__loongarch_asx` or `__loongarch_simd_width`.

The `__loongarch_asx_sx_conv` macro is defined when the compiler provides the LASX 128-bit lane helper intrinsics such as `__lasx_cast_128`, `__lasx_concat_128`, `__lasx_extract_128_lo` and `__lasx_insert_128_lo`. This macro is available in GCC 16 and Clang 22 and later.
