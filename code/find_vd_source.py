import glob
import re

# Update site directory before running: `poetry run mkdocs build`

for ext in ["lsx", "lasx"]:
    arg_counts = {}

    # find known insts from binutils-gdb
    for line in open("../../binutils-gdb/opcodes/loongarch-opc.c"):
        line = line.strip()
        if line.startswith("{") and line.endswith("},"):
            parts = line.split(",")
            if '"' in parts[2]:
                name = parts[2].split('"')[1]
                fmt = line.split('"')[3]

                arg_counts[name] = len(fmt.split(","))

    # find documented intrinsics
    for f in glob.glob(f"../site/{ext}/**/*.html", recursive=True):
        # forgive me to use the simple but fragile way to parse html
        for line in open(f, "r"):
            if "h2" in line:
                intrinsics = line.split(">")[1].split("<")[0]
                arg_count = len(intrinsics.split(","))
                if "__lsx_" in intrinsics:
                    intr_name = intrinsics[intrinsics.find("__lsx_") + 6 :].split(" ")[0]
                else:
                    intr_name = intrinsics[intrinsics.find("__lasx_") + 7 :].split(" ")[0]

                name = intr_name.replace("_", ".")

                # align with gen_measure.py
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

                if name.startswith("vst") or name.startswith("xvst"):
                    continue

                if name in arg_counts:
                    binutils_arg_count = arg_counts[name]
                    if arg_count + 1 != binutils_arg_count and vd_source == False:
                        print("Possible vd source not identified", intrinsics, name)
                    elif arg_count + 1 == binutils_arg_count and vd_source == True:
                        print("Possible wrong vd source", intrinsics, name)
                else:
                    print("Missing", name)
