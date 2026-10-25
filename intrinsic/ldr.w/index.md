# ldr.w

LBT intrinsic · group: [MIPS Memory](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/mips_memory/index.md)

### Synopsis

```
Instruction: ldr.w rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style load doubleword right: load right part of an unaligned doubleword/word from memory at address `rj + imm`, merge with the existing value in `rd`, store the result in `rd`.

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
