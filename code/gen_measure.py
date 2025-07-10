import glob
import re
import os

# find known insts from binutils-gdb
with open("measure.h", "w") as f:
    # measure unit time
    print('INSTR_TEST(unit, "add.w $r12, $r1, $r12\\n", "r12")', file=f)

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

                if name.startswith("v") or name.startswith("xv"):
                    print("Processing", name, fmt)

                    # handle instructions where vd/xd is also a source
                    # use find_vd_source.py
                    if (
                        re.match("x?vshuf\.[hwd]", name)
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
                    # skip vset/xvset because therid destination is vd
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
                            print(
                                f'INSTR_TEST({name.replace(".", "_")}_{depend_i}, "{name} {", ".join(ops)}\\n")',
                                file=f,
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
                        print(
                            f'INSTR_TEST8({name.replace(".", "_")}_tp, "{"".join(insts)}")',
                            file=f,
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
                        print(
                            f'INSTR_TEST({name.replace(".", "_")}_tp, "{name} {", ".join(ops)}\\n")',
                            file=f,
                        )

    # test undocumented intrinsics
    for name, opcode in [
        ("vfscaleb_s", "0x71448000"),
        ("vfscaleb_d", "0x71450000"),
        ("xvfscaleb_s", "0x75448000"),
        ("xvfscaleb_d", "0x75450000"),
    ]:
        # inst vr0, vr0, vr1
        print(
            f'INSTR_TEST({name}_1, ".word {opcode} + 0 + (0 << 5) + (1 << 10)\\n")',
            file=f,
        )

        # inst vr0, vr1, vr0
        print(
            f'INSTR_TEST({name}_2, ".word {opcode} + 0 + (1 << 5) + (0 << 10)\\n")',
            file=f,
        )

        # inst vr0, vr1, vr2
        print(
            f'INSTR_TEST({name}_tp, ".word {opcode} + 0 + (1 << 5) + (2 << 10)\\n")',
            file=f,
        )

os.system("clang-format -i measure.h")
