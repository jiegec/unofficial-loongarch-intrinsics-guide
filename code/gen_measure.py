import glob
import re
import os
from typing import Iterator


def detect_kind(name: str) -> str:
    if name.startswith("xv") or name.startswith("vext2xv"):
        return "lasx"
    if name.startswith("v"):
        return "lsx"
    return ""


# ── LBT detection ──────────────────────────────────────────────────────────

# Instructions that start with these prefixes are LBT
_LBT_PREFIXES = (
    "x86",
    "arm",
    "setx86j",
    "setx86loope",
    "setx86loopne",
    "setarmj",
    "adc.",
    "sbc.",
    "rotr.",
    "rotri.",
    "rcr.",
    "rcri.",
    "ldl.",
    "ldr.",
    "stl.",
    "str.",
    "addu12i.",
    "fcvt.ud.d",
    "fcvt.ld.d",
    "fcvt.d.ld",
    "movgr2scr",
    "movscr2gr",
    "jiscr0",
    "jiscr1",
)


def _is_lbt(name: str) -> bool:
    for pfx in _LBT_PREFIXES:
        if name.startswith(pfx):
            return True
    return False


def _lbt_stem(name: str) -> str:
    return name.split(".")[0] if "." in name else name


def _lbt_is_cond_field(stem: str, field: str) -> bool:
    if stem in {"setx86j", "setarmj"}:
        return field == "u10:4"
    if stem.startswith("arm"):
        return field in {"u0:4", "u10:4"}
    return False


def _lbt_fmt_parts(stem: str, fmt: str):
    """Split a LoongArch format string into typed parts.

    Returns list of ("kind", index) where kind is one of:
      rN  - GPR with bit-field index N (0=rd, 5=rj, 10=rk)
      crN - scratch register
      fN  - FPR
      imm - immediate / offset (carries no index)
      cond - ARM/x86 condition field
      complex - weird like "s0:5|10:16<<2"
    """
    out = []
    for p in fmt.split(","):
        p = p.strip()
        if not p:
            continue
        if "|" in p:
            out.append(("complex", 0))
        elif p.startswith("cr"):
            idx = int(p.split(":")[0][2:])
            out.append(("cr", idx))
        elif p.startswith("f"):
            idx = int(p.split(":")[0][1:])
            out.append(("f", idx))
        elif p.startswith("r"):
            idx = int(p.split(":")[0][1:])
            out.append(("r", idx))
        elif p.startswith(("u", "s")):
            if _lbt_is_cond_field(stem, p):
                out.append(("cond", 0))
            else:
                out.append(("imm", 0))
        else:
            out.append(("other", 0))
    return out


# ── LBT register allocation (ABI-aware) ───────────────────────────────────
# Latency chain: use $r12 (t0) as the single chained destination.

_LAT = "$r12"  # latency chain register
_TP_DST = [f"$r{i}" for i in range(12, 20)]  # t0..t7 (8 lanes)
_TP_SRC = [f"$r{i}" for i in range(4, 12)]  # a0..a7 (8 lanes)

# GPR direct latency chain.
_LBT_LATENCY_STEMS = {
    "rotr",
    "rotri",
    "addu12i",
    "adc",
    "sbc",
    "rcr",
    "rcri",
    "x86add",
    "x86sub",
    "x86and",
    "x86or",
    "x86xor",
    "x86mul",
    "armadd",
    "armsub",
    "armmove",
}
# flag latency chain, need setup before the measured loop.
_LBT_SETUP_LATENCY_STEMS = {"x86adc", "x86sbc", "armadc", "armsbc"}

_LBT_THROUGHPUT_STEMS = {
    "rotr",
    "rotri",
    "movgr2scr",
    "movscr2gr",
    "addu12i",
    "adc",
    "sbc",
    "rcr",
    "rcri",
    "fcvt",
    "x86add",
    "x86sub",
    "x86and",
    "x86or",
    "x86xor",
    "x86mul",
    "x86adc",
    "x86sbc",
    "setx86j",
    "x86mfflag",
    "x86mtflag",
    "x86mftop",
    "x86mttop",
    "x86inctop",
    "x86dectop",
    "x86settm",
    "x86clrtm",
    "armadd",
    "armsub",
    "armadc",
    "armsbc",
    "armmove",
    "armmfflag",
    "armmtflag",
    "setarmj",
}


def _lbt_cond_val(stem: str) -> str:
    """Return the condition field value for ARM/x86 helpers."""
    arm = stem.startswith("arm") or stem == "setarmj"
    if arm:
        return "14"  # AL: avoid condition-flag deps
    return "4"  # ZF for x86 helpers


def _lbt_imm_val(name, stem):
    """Return the immediate field value for a given instruction."""
    if name.startswith(("x86mfflag", "x86mtflag")):
        return "0x3f"
    if name.startswith(("armmfflag", "armmtflag")):
        return "0x39"
    if stem in {"setx86j", "setarmj"}:
        return "4"
    return "1"


def _lbt_emit_asm(name, stem, parts, reg_alloc, lane=0):
    """Build one asm string from parsed format parts.

    reg_alloc(gpr_idx, lane) → register string (e.g. "$r12" or "$r4").
    """
    ops = []
    gpr_idx = 0
    for kind, idx in parts:
        if kind == "r":
            ops.append(reg_alloc(gpr_idx, lane))
            gpr_idx += 1
        elif kind == "cond":
            ops.append(_lbt_cond_val(stem))
        elif kind == "imm":
            ops.append(_lbt_imm_val(name, stem))
        elif kind == "f":
            ops.append(f"$f{idx}")
        elif kind == "cr":
            ops.append("$scr0")
        else:
            ops.append("0")
    return f"{name} {', '.join(ops)}"


def _lbt_lat_reg(chained_indices):
    """Return a reg_alloc that chains $r12 through given GPR indices, $r13 otherwise."""

    def _reg(gpr_idx, _lane):
        return _LAT if gpr_idx in chained_indices else "$r13"

    return _reg


class TestsCollector:
    def __init__(self) -> None:
        self._data: dict[str, list[list[str]]] = {
            "": [],
            "lsx": [],
            "lasx": [],
            "lbt": [],
        }

    def instr_test(
        self,
        kind: str,
        name: str,
        *params: str,
    ) -> None:
        self._instr_test(kind, "INSTR_TEST", name, *params)

    def instr_test8(
        self,
        kind: str,
        name: str,
        *params: str,
    ) -> None:
        self._instr_test(kind, "INSTR_TEST8", name, *params)

    def instr_test_setup(
        self,
        kind: str,
        name: str,
        *params: str,
    ) -> None:
        self._instr_test(kind, "INSTR_TEST_SETUP", name, *params)

    def _instr_test(
        self,
        kind: str,
        macro: str,
        name: str,
        *params: str,
    ) -> None:
        l = [macro, name]
        l.extend(params)
        self._data[kind].append(l)

    def emit_lines(self) -> Iterator[str]:
        for kind, val in self._data.items():
            yield from self._emit_kind(kind, val)

    def _emit_kind(self, kind: str, val: list[list[str]]) -> Iterator[str]:
        gate_macro = f"MACHINE_HAS_{kind.upper()}" if kind else ""
        if gate_macro:
            yield f"#if {gate_macro}"

        for l in val:
            macro, name = l[:2]
            params = ", ".join(l[2:])
            yield f"{macro}({name}, {params})"

        if gate_macro:
            yield f"#endif  /* {gate_macro} */"


# find known insts from binutils-gdb
with open("measure.h", "w") as f:
    print("/* This file is auto-generated by gen_measure.py. DO NOT EDIT! */", file=f)

    col = TestsCollector()

    # measure unit time
    col.instr_test("", "unit", '"add.w $r12, $r1, $r12\\n"', '"r12"')

    for line in open("../../binutils-gdb/opcodes/loongarch-opc.c"):
        line = line.strip()
        if line.startswith("{") and line.endswith("},"):
            parts = line.split(",")
            if '"' in parts[2]:
                name = parts[2].split('"')[1]
                fmt = line.split('"')[3]

                if (
                    name.startswith("vld")
                    or name.startswith("vst")
                    or name.startswith("xvld")
                    or name.startswith("xvst")
                    or "frsqrte" in name
                    or "frecipe" in name
                ):
                    continue

                # ── SIMD ───────
                if name.startswith("v") or name.startswith("xv"):
                    kind = detect_kind(name)
                    print("Processing", name, fmt)

                    # handle instructions where vd/xd is also a source
                    # use find_vd_source.py
                    if (
                        re.match("x?vshuf\\.[hwd]", name)
                        or re.match("x?vm(add|sub).*", name)
                        or re.match("x?vextrins.*", name)
                        or re.match("x?vfrstp.*", name)
                        or re.match("x?vinsve0.*", name)
                        or re.match("x?vinsgr2vr.*", name)
                        or re.match("x?vs?(srl|sra)r?ni.*", name)
                        or name == "xvshuf4i.d"
                        or name == "xvpermi.w"
                        or name == "xvpermi.q"
                        or name == "xvbitseli.b"
                        or name == "vbitseli.b"
                        or name == "vpermi.w"
                        or name == "vshuf4i.d"
                    ):
                        vd_source = True
                    else:
                        vd_source = False

                    # latency test
                    # at least one op depends on vd if vd_source is False
                    fmt_parts = fmt.split(",")
                    if vd_source:
                        depend_i_begin = 0
                    else:
                        depend_i_begin = 1
                    # skip vset/xvset because their destination is vd
                    if not (name.startswith("vset") or name.startswith("xvset")):
                        for depend_i in range(depend_i_begin, len(fmt_parts)):
                            if not (
                                fmt_parts[depend_i].startswith("v")
                                or fmt_parts[depend_i].startswith("x")
                            ):
                                # not a real dependency
                                continue

                            ops = []
                            for i, part in enumerate(fmt_parts):
                                if part.startswith("v"):
                                    if i == depend_i:
                                        # vd is always vr0
                                        ops.append("$vr0")
                                    else:
                                        ops.append(f"$vr{i}")
                                elif part.startswith("x"):
                                    if i == depend_i:
                                        # vd is always vr0
                                        ops.append("$xr0")
                                    else:
                                        ops.append(f"$xr{i}")
                                elif part.startswith("r"):
                                    ops.append(f"$r0")
                                elif part.startswith("c"):
                                    ops.append(f"$fcc0")
                                elif part.startswith("u") or part.startswith("s"):
                                    # imm
                                    ops.append("0")
                            col.instr_test(
                                kind,
                                f'{name.replace(".", "_")}_{depend_i}',
                                f'"{name} {", ".join(ops)}\\n"',
                            )

                    # throughput test
                    # no dependency
                    if vd_source:
                        # repeat unrelated sequence 8 times
                        insts = []
                        for j in range(8):
                            ops = []
                            for i, part in enumerate(fmt_parts):
                                i += j * 4
                                if part.startswith("v"):
                                    ops.append(f"$vr{i}")
                                elif part.startswith("x"):
                                    ops.append(f"$xr{i}")
                                elif part.startswith("r"):
                                    ops.append(f"$r0")
                                elif part.startswith("c"):
                                    ops.append(f"$fcc0")
                                elif part.startswith("u") or part.startswith("s"):
                                    # imm
                                    ops.append("0")
                            insts.append(f'{name} {", ".join(ops)}\\n')
                        col.instr_test8(
                            kind,
                            f'{name.replace(".", "_")}_tp',
                            f'"{"".join(insts)}"',
                        )
                    else:
                        ops = []
                        for i, part in enumerate(fmt_parts):
                            if part.startswith("v"):
                                ops.append(f"$vr{i}")
                            elif part.startswith("x"):
                                ops.append(f"$xr{i}")
                            elif part.startswith("r"):
                                ops.append(f"$r0")
                            elif part.startswith("c"):
                                ops.append(f"$fcc0")
                            elif part.startswith("u") or part.startswith("s"):
                                # imm
                                ops.append("0")
                        col.instr_test(
                            kind,
                            f'{name.replace(".", "_")}_tp',
                            f'"{name} {", ".join(ops)}\\n"',
                        )

                # ── LBT ─────────────────────────────────────────────
                elif _is_lbt(name):
                    stem = _lbt_stem(name)

                    # Skip control-flow instructions (can't microbench)
                    # Skip MIPS unaligned memory helpers (ldl/ldr/stl/str)
                    # Skip x87 tag helper (needs x87 state init)
                    if stem in {
                        "jiscr0",
                        "jiscr1",
                        "ldl",
                        "ldr",
                        "stl",
                        "str",
                        "x86settag",
                    }:
                        continue

                    print("Processing LBT", name, fmt)
                    safe_name = name.replace(".", "_")
                    lbt_parts = _lbt_fmt_parts(stem, fmt)
                    gpr_count = sum(1 for k, _ in lbt_parts if k == "r")
                    has_gpr_output = (
                        stem not in {"x86mtflag", "armmtflag"}
                        and lbt_parts
                        and lbt_parts[0][0] == "r"
                        and lbt_parts[0][1] == 0
                    )

                    # Flag latency tests with one-time setup before the loop.
                    if stem in _LBT_SETUP_LATENCY_STEMS and gpr_count == 2:
                        flag_setup = (
                            "x86mtflag $r0, 0x1\\n"
                            if stem.startswith("x86")
                            else "armmtflag $r0, 0x1\\n"
                        )
                        if stem in {"x86adc", "armadc"}:
                            rj_setup = f"addi.d {_LAT}, $r0, -1\\n"
                        else:
                            rj_setup = f"addi.d {_LAT}, $r0, 0\\n"
                        setup = f"{rj_setup}addi.d $r13, $r0, 0\\n{flag_setup}"
                        asm = _lbt_emit_asm(
                            name,
                            stem,
                            lbt_parts,
                            _lbt_lat_reg({0}),
                            lane=-1,
                        )
                        col.instr_test_setup(
                            "lbt",
                            f"{safe_name}_0",
                            f'"{setup}"',
                            f'"{asm}\\n"',
                            '"r12", "r13", "memory"',
                        )
                        # Also test both GPRs chained (_1: all=$r12)
                        asm_all = _lbt_emit_asm(
                            name,
                            stem,
                            lbt_parts,
                            _lbt_lat_reg({0, 1}),
                            lane=-1,
                        )
                        col.instr_test_setup(
                            "lbt",
                            f"{safe_name}_1",
                            f'"{setup}"',
                            f'"{asm_all}\\n"',
                            '"r12", "r13", "memory"',
                        )

                    # Direct GPR RAW latency tests.
                    if stem in _LBT_LATENCY_STEMS:
                        # Test each source GPR individually chained to rd
                        source_indices = list(range(1, gpr_count))
                        for src_i in source_indices:
                            asm = _lbt_emit_asm(
                                name,
                                stem,
                                lbt_parts,
                                _lbt_lat_reg({0, src_i}),
                            )
                            clob = '"r12", "r13"'
                            col.instr_test(
                                "lbt",
                                f"{safe_name}_{src_i}",
                                f'"{asm}\\n"',
                                clob,
                            )

                        # Also test all sources chained (when >1 source GPR)
                        if len(source_indices) > 1:
                            asm = _lbt_emit_asm(
                                name,
                                stem,
                                lbt_parts,
                                _lbt_lat_reg(set(range(gpr_count))),
                            )
                            col.instr_test(
                                "lbt",
                                f"{safe_name}_3",
                                f'"{asm}\\n"',
                                '"r12"',
                            )

                    # Latency tests for FPR/GPR cross-domain instructions
                    if name == "fcvt.ud.d" or name == "fcvt.ld.d":
                        # fd=FPR, fj=FPR — chain through FPR $f13
                        asm = f"{name} $f13, $f13"
                        col.instr_test(
                            "lbt",
                            f"{safe_name}_1",
                            f'"{asm}\\n"',
                            '"f13"',
                        )
                    elif name == "fcvt.d.ld":
                        # format f0:5,f5:5,f10:5 — all FPRs (fd, fj, fk)
                        # Chain through fj ($f12 same as fd → RAW)
                        col.instr_test(
                            "lbt",
                            f"{safe_name}_1",
                            f'"fcvt.d.ld $f12, $f12, $f13\\n"',
                            '"f12", "f13"',
                        )
                        # Chain through fk
                        col.instr_test(
                            "lbt",
                            f"{safe_name}_2",
                            f'"fcvt.d.ld $f12, $f13, $f12\\n"',
                            '"f12", "f13"',
                        )
                        # Both sources chained
                        col.instr_test(
                            "lbt",
                            f"{safe_name}_3",
                            f'"fcvt.d.ld $f12, $f12, $f12\\n"',
                            '"f12"',
                        )

                    # Throughput test
                    tp_asms = None
                    if stem in _LBT_THROUGHPUT_STEMS:
                        if not fmt.strip():
                            tp_asms = [name] * 8
                        elif lbt_parts and lbt_parts[0][0] == "f":
                            tp_asms = [
                                _lbt_emit_asm(
                                    name, stem, lbt_parts, lambda _gpr_idx, _lane: "$r0"
                                )
                            ]
                        elif lbt_parts and lbt_parts[0][0] == "cr":
                            tp_asms = [
                                _lbt_emit_asm(
                                    name,
                                    stem,
                                    lbt_parts,
                                    lambda _gpr_idx, _lane: "$r12",
                                )
                            ]
                        elif all(k == "imm" for k, _ in lbt_parts):
                            tp_asms = [f"{name} 0"]
                        else:

                            def _tp_reg(gpr_idx, lane):
                                if gpr_idx == 0:
                                    return _TP_DST[lane]
                                return _TP_SRC[lane]

                            tp_asms = [
                                _lbt_emit_asm(name, stem, lbt_parts, _tp_reg, lane=ln)
                                for ln in range(8)
                            ]

                    if tp_asms is not None:
                        tp_str = '"' + "".join(f"{a}\\n" for a in tp_asms) + '"'
                        clob = (
                            ", ".join(f'"{r[1:]}"' for r in _TP_DST)
                            if has_gpr_output
                            else ""
                        )
                        if len(tp_asms) == 8:
                            # 8 independent lanes → INSTR_TEST8
                            add_test = col.instr_test8
                        else:
                            # Single lane → INSTR_TEST
                            add_test = col.instr_test

                        if clob:
                            add_test(
                                "lbt",
                                f"{safe_name}_tp",
                                tp_str,
                                clob,
                            )
                        else:
                            add_test(
                                "lbt",
                                f"{safe_name}_tp",
                                tp_str,
                            )

    # test undocumented intrinsics
    for name, opcode in [
        ("vfscaleb_s", "0x71448000"),
        ("vfscaleb_d", "0x71450000"),
        ("xvfscaleb_s", "0x75448000"),
        ("xvfscaleb_d", "0x75450000"),
    ]:
        kind = detect_kind(name)

        # inst vr0, vr0, vr1
        col.instr_test(
            kind,
            f"{name}_1",
            f'".word {opcode} + 0 + (0 << 5) + (1 << 10)\\n"',
        )

        # inst vr0, vr1, vr0
        col.instr_test(
            kind,
            f"{name}_2",
            f'".word {opcode} + 0 + (1 << 5) + (0 << 10)\\n"',
        )

        # inst vr0, vr1, vr2
        col.instr_test(
            kind,
            f"{name}_tp",
            f'".word {opcode} + 0 + (1 << 5) + (2 << 10)\\n"',
        )

    for name, opcode in [
        ("vmepatmsk_v", "0x729b8000"),
        ("xvmepatmsk_v", "0x769b8000"),
    ]:
        kind = detect_kind(name)

        # inst xr0, 0, 0
        col.instr_test(
            kind,
            f"{name}_tp",
            f'".word {opcode} + 0 + (0 << 5) + (0 << 10)\\n"',
        )

    for line in col.emit_lines():
        print(line, file=f)

os.system("clang-format -i measure.h")
