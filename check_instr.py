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
                fmt = parts[3].split('"')[1]
                encoding[name] = fmt
                print(name, fmt)

    # find documented intrinsics
    for f in glob.glob(f"./site/{ext}/**/*.html", recursive=True):
        # forgive me to use the simple but fragile way to parse html
        for line in open(f, "r"):
            if line.startswith("Instruction:"):
                # check instruction format
                inst = line[12:].strip()
                name = inst.split(" ")[0]
                fmt = encoding[name]
                print(inst, fmt)