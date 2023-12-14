import glob

# Update site directory before running: `poetry run mkdocs build`

for ext in ["lsx", "lasx"]:
    encoding = {}

    # find known insts from binutils-gdb
    for line in open("../binutils-gdb/opcodes/loongarch-opc.c"):
        line = line.strip()
        if line.startswith("{") and line.endswith("},"):
            parts = line.split(",")
            if '"' in parts[2]:
                name = parts[2].split('"')[1]
                fmt = line.split('"')[3]

                # convert fmt to our naming
                encoded = []
                for part in fmt.split(","):
                    if len(part) == 0:
                        continue
                    if part[0] == "x":
                        encoded.append("xr")
                    elif part[0] == "v":
                        encoded.append("vr")
                    elif part[0] == "r":
                        encoded.append("r")
                    elif part[0] == "u" or part[0] == "s":
                        encoded.append("imm")
                encoding[name] = ", ".join(encoded)
                # print(name, ", ".join(encoded))

    # find documented intrinsics
    for f in glob.glob(f"./site/{ext}/**/*.html", recursive=True):
        # forgive me to use the simple but fragile way to parse html
        for line in open(f, "r"):
            if line.startswith("Instruction:"):
                # check instruction format
                inst = line[12:].strip()
                name = inst.split(" ")[0]
                fmt = encoding[name]

                args = " ".join(inst.split(";")[0].split(" ")[1:])
                if args != fmt:
                    print("Mismatch inst=", inst, ",args=", args, ",fmt=", fmt)