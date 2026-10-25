# jiscr1

LBT intrinsic · group: [LBT Jump](https://jia.je/unofficial-loongarch-intrinsics-guide/lbt/jump/index.md)

### Synopsis

```
Instruction: jiscr1 imm
CPU Flags: LBT
```

### Description

Jump via scratch register 1: save the current PC to SCR[0], then add `imm` to the value in SCR[1] and jump to the resulting address.

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
