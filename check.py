import glob

# Update site directory before running: `poetry run mkdocs build`

for ext in ["lsx", "lasx"]:

    def parse_fn(line, skip_last):
        before_paren = line.split("(")[0].split(" ")
        between_parens = line.split("(")[1].split(")")[0]
        args = between_parens.split(",")

        result = before_paren
        result.append("(")
        for arg in args:
            if skip_last:
                result += arg.split(" ")[:-1]
            else:
                result += arg.split(" ")
        result.append(")")

        return tuple(result)

    # gcc intrinsics
    gcc_intrinsics = set()
    for line in open(f"gcc_{ext}intrin.h", "r"):
        gcc_intrinsics.add(parse_fn(line, False))

    # find documented intrinsics
    documented_intrinsics = set()
    for f in glob.glob(f"./site/{ext}/**/*.html", recursive=True):
        # forgive me to use the simple but fragile way to parse html
        for line in open(f, "r"):
            if "h2" in line:
                intrinsics = line.split(">")[1].split("<")[0]
                documented_intrinsics.add(parse_fn(intrinsics, True))

    undocumented = gcc_intrinsics - documented_intrinsics
    for entry in undocumented:
        print("Undocumented:", " ".join(entry))
        for e in documented_intrinsics:
            if e[1] == entry[1]:
                print("Matching:", " ".join(e))
                print(repr(e))
                print(repr(entry))

    extra = documented_intrinsics - gcc_intrinsics
    for entry in extra:
        print("Extra:", " ".join(entry))
