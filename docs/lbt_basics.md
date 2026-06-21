# LoongArch LBT Basics

LBT (LoongArch Binary Translation) is a set of instructions for emulating x86 and ARM instruction sets. It provides hardware-accelerated flag manipulation, x87 FPU stack management, and conditional execution primitives.

## Registers

5 dedicated registers shared with the floating-point unit:

| Register  | Purpose                                     |
|-----------|---------------------------------------------|
| LBT0–LBT3 | Scratch registers (GPR save/restore)        |
| LBT4      | High 32 bits: x87 FTOP, Low 32 bits: EFLAGS |

Scratch registers are accessible via:

- `$scr0` = LBT0
- `$scr1` = LBT1
- `$scr2` = LBT2
- `$scr3` = LBT3

```c
// movgr2scr $scrd, $rj
movgr2scr(scrd, rj) {
    SCR[scrd] = GPR[rj];
}

// movscr2gr $rd, $scrj
movscr2gr(rd, scrj) {
    GPR[rd] = SCR[scrj];
}
```

### jiscr

```c
// jiscr0 imm
jiscr0(imm) {
    PC = SCR[0] + imm;
}

// jiscr1 imm
jiscr1(imm) {
    SCR[0] = PC;
    PC = SCR[1] + imm;
}
```

## x86 EFLAGS

EFLAGS bits are stored in the low 32 bits of LBT4:

| Bit | Flag  | Position Shift |
|-----|-------|----------------|
| CF  | 0x001 | 0              |
| PF  | 0x004 | 2              |
| AF  | 0x010 | 4              |
| ZF  | 0x040 | 6              |
| SF  | 0x080 | 7              |
| OF  | 0x800 | 11             |

All flags set: `0x8d5`

## ARMFLAGS

ARM condition flags (N, Z, C, V) are mapped to the corresponding x86 EFLAGS bits in LBT4:

| ARM Flag     | x86 Equivalent | Bit Position |
|--------------|----------------|--------------|
| N (Negative) | SF             | 0x080        |
| Z (Zero)     | ZF             | 0x040        |
| C (Carry)    | CF             | 0x001        |
| V (Overflow) | OF             | 0x800        |

## x87 FPU Emulation

### FTOP (Stack Top Pointer)

The x87 FPU uses an 8-entry stack. FTOP (3-bit, stored in high 32 bits of LBT4) points to the current stack top. FPR register indices are remapped by TOP when TM mode is enabled.

```c
// x86mftop $rd
x86mftop(rd) {
    GPR[rd] = TOP;
}

// x86mttop imm
x86mttop(imm) {
    TOP = imm;
}

// x86inctop (FINCSTP)
x86inctop() {
    TOP = (TOP + 1) & 0x7;
}

// x86dectop (FDECSTP)
x86dectop() {
    TOP = (TOP - 1) & 0x7;
}
```

### TM (Translation Mode)

When TM is set, FPR indices 0-7 are offset by TOP:

```c
// x86settm
x86settm() {
    TM = 1;
}

// x86clrtm
x86clrtm() {
    TM = 0;
}

mappedFPR(fpr) {
    if (TM && fpr < 8) {
        return FPR[(fpr + TOP) % 8];
    } else {
        return FPR[fpr];
    }
}
```

LSX/LASX registers are not remapped.

### x87 80-bit Extended Precision

x87 long double (80-bit) is stored across two FPRs:

| Part         | Format                             | Register                      |
|--------------|------------------------------------|-------------------------------|
| Low 64 bits  | J(bit 63) + fraction(bits 62:0)    | fd                            |
| High 16 bits | sign(bit 15) + exponent(bits 14:0) | upper bits of paired register |

Conversion instructions:

- `fcvt.ld.d fd, fj` — double → low 64 bits of extended
- `fcvt.ud.d fd, fj` — double → high 16 bits of extended
- `fcvt.d.ld fd, fj, fk` — extended → double

### Tag Word

`x86settag rd, imm1, imm2` manipulates the x87 tag word (stored in a GPR). The operation mode is encoded in `imm1 % 8`:

- 0: set bit (0→1 only), raises BTE on invalid transition
- 1: clear bit (1→0 only), raises BTE on invalid transition
- 2–4: check tag byte then modify, raises BTE on invalid state

## Instruction Groups

All LBT instruction groups are documented in the following pages:

- [LBT Integer](lbt/integer.md): `addu12i`, `adc`, `sbc`, `rotr`, `rotri`, `rcr`, `rcri`
- [x86 Integer](lbt/x86_integer.md): `x86adc`, `x86add`, `x86inc`, `x86sbc`, `x86sub`, `x86dec`, `x86mul`
- [x86 Logical](lbt/x86_logical.md): `x86and`, `x86or`, `x86xor`
- [x86 Shift](lbt/x86_shift.md): `x86rcl`, `x86rcli`, `x86rcr`, `x86rcri`, `x86rotl`, `x86rotli`, `x86rotr`, `x86rotri`, `x86sll`, `x86slli`, `x86srl`, `x86srli`, `x86sra`, `x86srai`
- [x86 FPU](lbt/x86_fpu.md): `x86mftop`, `x86mttop`, `x86inctop`, `x86dectop`, `x86settm`, `x86clrtm`, `x86settag`, `fcvt.ud.d`, `fcvt.ld.d`, `fcvt.d.ld`
- [x86 Misc](lbt/x86_misc.md): `setx86j`, `setx86loope`, `setx86loopne`
- [ARM Integer](lbt/arm_integer.md): `armadd`, `armsub`, `armadc`, `armsbc`
- [ARM Logical](lbt/arm_logical.md): `armand`, `armor`, `armxor`, `armnot`
- [ARM Shift](lbt/arm_shift.md): `armsll`, `armsrl`, `armsra`, `armrotr`, `armslli`, `armsrli`, `armsrai`, `armrotri`
- [ARM Misc](lbt/arm_misc.md): `armrrx`, `armmove`, `armmov`, `setarmj`
- [LBT State Transfer](lbt/state.md): `movgr2scr`, `movscr2gr`, `x86mfflag`, `x86mtflag`, `armmfflag`, `armmtflag`
- [LBT Jump](lbt/jump.md): `jiscr0`, `jiscr1`
- [MIPS Memory](lbt/mips_memory.md): `ldl.d`, `ldl.w`, `ldr.w`, `ldr.d`, `stl.w`, `stl.d`, `str.w`, `str.d`
