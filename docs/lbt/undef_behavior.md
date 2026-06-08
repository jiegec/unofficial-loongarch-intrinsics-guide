# Undefined Flag Behavior Reference

The x86 architecture leaves certain flag bits **undefined** after various instructions.
Real CPUs handle these differently across models and generations.
LBT (LoongArch Binary Translation) makes specific choices for each.

This page compares LBT against real x86 CPUs where probe data is available.

The table below is measured from `PROBE|...` output, not inferred from the
metadata comments in the pseudocode headers. Metadata is useful for generation,
but it is not treated as authoritative here.

## Behavior Codes

| Code | Meaning |
|------|---------|
{{ legend_table() }}

## Per-Instruction Comparison

{{ lbt_undef_table() }}

## Defined-Behavior LBT Errata

The following cases are not x86 undefined-flag behavior. They are defined x86
semantics where tested LBT hardware differs from native x86, so translators must
handle them explicitly.

{{ lbt_errata_table() }}

## Notes

- **`?`** means not yet measured on that CPU.
- The current automated probe covers LBT x86 flag instructions implemented in this repository: logic, `mul`, shifts, rotates, `rcl`, and `rcr`. The native x86 probe also covers probe-only families for `test`, `bt/btc/btr/bts`, `bsf/bsr`, `div/idiv`, and `shld/shrd`. Legacy `aaa/aas/aam/aad/daa/das` and x87 `fcomi/fucomi/fcomip/fucomip` remain separate probe work.
- Native x86 reports contain `CHECK|FAIL` lines for `x86rotl/x86rotr` count-zero and count-width cases. Those are the defined-behavior errata listed above; they are not undefined-flag probe failures.
- Reports used for this snapshot: `code/lbt/reports/lbt-local-loongarch64.txt` on Loongson-3C6000/S LA664, `code/lbt/reports/x86-intel-g1610t.txt`, and `code/lbt/reports/x86-amd-zen3-5950x.txt`.
- Scope reference: [sandpile.org x86 flags](https://www.sandpile.org/x86/flags.htm).

*Last updated: auto-generated from `code/lbt/undef_behaviors.yaml` and `code/lbt/errata.yaml`*
