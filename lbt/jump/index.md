# LBT Jump

## jiscr0

### Synopsis

```
Instruction: jiscr0 imm
CPU Flags: LBT
```

### Description

Jump via scratch register 0: add `imm` to the value in SCR[0] and jump to the resulting address.

## jiscr1

### Synopsis

```
Instruction: jiscr1 imm
CPU Flags: LBT
```

### Description

Jump via scratch register 1: save the current PC to SCR[0], then add `imm` to the value in SCR[1] and jump to the resulting address.
