import glob

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

                    # latency test
                    # at least one op depends on vd
                    fmt_parts = fmt.split(",")
                    for depend_i in range(1, len(fmt_parts)):
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
