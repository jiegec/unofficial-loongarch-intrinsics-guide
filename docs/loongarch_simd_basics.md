# LoongArch SIMD Basics

## Registers and Types

LoongArch SIMD has two vector extensions:

- **LSX** (LoongArch SIMD eXtension): 128-bit vectors in registers `v0`-`v31`
- **LASX** (LoongArch Advanced SIMD eXtension): 256-bit vectors in registers `x0`-`x31`

The C/C++ intrinsic types are:

| Width          | Integer   | Float    | Double    |
|----------------|-----------|----------|-----------|
| 128-bit (LSX)  | `__m128i` | `__m128` | `__m128d` |
| 256-bit (LASX) | `__m256i` | `__m256` | `__m256d` |

In hardware, the lower bits of FP/LSX/LASX registers are shared:

- FP register `f0`, LSX register `v0`, and LASX register `x0` share the lowest 64 bits
- LSX register `v0` and LASX register `x0` share the lowest 128 bits

## Element Type Suffixes

Instruction mnemonics and intrinsics use consistent suffixes to indicate element width and signedness:

| Suffix | Meaning                      |
|--------|------------------------------|
| `b`    | 8-bit byte (signed)          |
| `bu`   | 8-bit byte (unsigned)        |
| `h`    | 16-bit halfword (signed)     |
| `hu`   | 16-bit halfword (unsigned)   |
| `w`    | 32-bit word (signed)         |
| `wu`   | 32-bit word (unsigned)       |
| `d`    | 64-bit doubleword (signed)   |
| `du`   | 64-bit doubleword (unsigned) |
| `q`    | 128-bit quadword (signed)    |
| `qu`   | 128-bit quadword (unsigned)  |
| `s`    | single-precision float       |
| `d`    | double-precision float       |

For example, `vadd_b` adds 16 signed bytes, while `vadd_w` adds 4 signed 32-bit integers.

## Instruction Prefix Convention

- LSX instructions use the `v` prefix (e.g., `vadd`, `vshuf`)
- LASX instructions use the `xv` prefix (e.g., `xvadd`, `xvshuf`)

This pattern holds across all categories: arithmetic, logical, memory, shuffle, etc.

## Compiler Macros

GCC and Clang define feature macros for LoongArch SIMD when the corresponding target options are enabled:

- `__loongarch_sx`: LSX is enabled, for example with `-mlsx`.
- `__loongarch_asx`: LASX is enabled, for example with `-mlasx`.
- `__loongarch_simd_width`: the enabled SIMD vector width in bits. It is `128` for LSX and `256` for LASX.

GCC also defines `__loongarch_simd` when some LoongArch SIMD extension is enabled. Clang does not define this macro, so portable code should prefer `__loongarch_sx`, `__loongarch_asx` or `__loongarch_simd_width`.

The `__loongarch_asx_sx_conv` macro is defined when the compiler provides the LASX 128-bit lane helper intrinsics such as `__lasx_cast_128`, `__lasx_concat_128`, `__lasx_extract_128_lo` and `__lasx_insert_128_lo`. This macro is available in GCC 16 and Clang 22 and later.

## Useful References

- [SSE to LSX migration guide](migrating_sse.md)
- [AVX to LASX migration guide](migrating_avx.md)
- [Latency and throughput tables](latency_throughput.md)
- [Interactive intrinsic browser](viewer.md)
