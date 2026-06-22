# MIPS Memory

## ldl.w

### Synopsis

```
Instruction: ldl.w rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style load doubleword left: load left part of an unaligned doubleword/word from memory at address `rj + imm`, merge with the existing value in `rd`, store the result in `rd`.

## ldl.d

### Synopsis

```
Instruction: ldl.d rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style load doubleword left: load left part of an unaligned doubleword/word from memory at address `rj + imm`, merge with the existing value in `rd`, store the result in `rd`.

## ldr.w

### Synopsis

```
Instruction: ldr.w rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style load doubleword right: load right part of an unaligned doubleword/word from memory at address `rj + imm`, merge with the existing value in `rd`, store the result in `rd`.

## ldr.d

### Synopsis

```
Instruction: ldr.d rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style load doubleword right: load right part of an unaligned doubleword/word from memory at address `rj + imm`, merge with the existing value in `rd`, store the result in `rd`.

## stl.w

### Synopsis

```
Instruction: stl.w rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style store doubleword left: store left part of value in `rd` to unaligned memory address `rj + imm`.

## stl.d

### Synopsis

```
Instruction: stl.d rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style store doubleword left: store left part of value in `rd` to unaligned memory address `rj + imm`.

## str.w

### Synopsis

```
Instruction: str.w rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style store doubleword right: store right part of value in `rd` to unaligned memory address `rj + imm`.

## str.d

### Synopsis

```
Instruction: str.d rd, rj, imm
CPU Flags: LBT
```

### Description

MIPS-style store doubleword right: store right part of value in `rd` to unaligned memory address `rj + imm`.
