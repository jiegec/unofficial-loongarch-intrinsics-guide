# x86settag

LBT intrinsic · group: [x86 FPU](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/x86_fpu/index.md)

### Synopsis

```
Instruction: x86settag rd, imm1, imm2
CPU Flags: LBT
```

### Description

Set x87 tag word bit in `rd` according to `imm1` and `imm2`. `imm2` selects the target bit position. `imm1%8` determines the operation: 0=set bit (0→1 only), 1=clear bit (1→0 only), 2-4=check tag byte then modify. Raises a BTE (Binary Translation Exception) on invalid state transitions.

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
