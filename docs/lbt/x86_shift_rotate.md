# LBT x86 Shift and Rotate Flags

These helpers generate x86 shift and rotate flags while preserving the operand GPRs.

The rotate helpers have a documented LA664 erratum for masked count zero. See
the per-instruction Errata sections below and the
[Undefined Flags](undef_behavior.md#defined-behavior-lbt-errata) page for the
summary table.

{{ lbt_page('x86_shift_rotate') }}
