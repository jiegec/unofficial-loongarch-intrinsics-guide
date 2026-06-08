# LBT ARM Helpers

ARM helpers use the ARM NZCV view of the shared LBT flag state and generally preserve GPR operands while generating condition or flag results. The `cond` operand is always a 4-bit ARM condition-code immediate. On the tested LBT hardware, `cond=15` behaves as always true, the same as `cond=14`.

## ARM condition enum

| cond | Mnemonic | Condition |
|------|----------|-----------|
| 0 | EQ | `Z == 1` |
| 1 | NE | `Z == 0` |
| 2 | CS/HS | `C == 1` |
| 3 | CC/LO | `C == 0` |
| 4 | MI | `N == 1` |
| 5 | PL | `N == 0` |
| 6 | VS | `V == 1` |
| 7 | VC | `V == 0` |
| 8 | HI | `C == 1 && Z == 0` |
| 9 | LS | `C == 0 || Z == 1` |
| 10 | GE | `N == V` |
| 11 | LT | `N != V` |
| 12 | GT | `Z == 0 && N == V` |
| 13 | LE | `Z == 1 || N != V` |
| 14 | AL | Always true |
| 15 | LBT AL | Always true on tested LBT hardware |

## ARM immediate fields

| Instruction family | Encoded fields | Meaning |
|--------------------|----------------|---------|
| `armadd.w`, `armsub.w`, `armadc.w`, `armsbc.w`, `armand.w`, `armor.w`, `armxor.w`, `armsll.w`, `armsrl.w`, `armsra.w`, `armrotr.w` | `rj`, `rk`, `cond:u4` | `cond` uses the table above. Register shift counts use `rk[4:0]`. |
| `armslli.w`, `armsrli.w`, `armsrai.w`, `armrotri.w` | `rj`, `imm:u5`, `cond:u4` | `imm` is a 5-bit shift/rotate count, range 0..31. |
| `armnot.w`, `armrrx.w`, `armmov.w`, `armmov.d`, `setarmj` | `rj` or `rd`, `cond:u4` | The only immediate is the condition enum. `armrrx.w` always rotates by one through `C`. |
| `armmove` | `rd`, `rj`, `cond:u4` | Conditional 64-bit move; false path preserves `rd`. |

When `cond` is false, the tested ARM helpers preserve their destination GPRs and preserve `NZCV`. `armmove` also preserves `rd`.

{{ lbt_page('arm') }}
