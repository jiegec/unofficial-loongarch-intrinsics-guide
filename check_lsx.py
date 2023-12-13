import os
import glob

# Initial worktree:
# git worktree add -f ../gh-pages gh-pages
# Update gh-pages before running:
# git fetch origin gh-pages && git -C ../gh-pages reset origin/gh-pages --hard

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
for line in open('gcc_lsxintrin.h', 'r'):
    gcc_intrinsics.add(parse_fn(line, False))
#print(gcc_intrinsics)

# find documented intrinsics
documented_intrinsics = set()
for f in glob.glob("../gh-pages/**/*.html", recursive=True):
    # forgive me to use the simple but fragile way to parse html
    for line in open(f, 'r'):
        if "h2" in line:
            intrinsics = line.split(">")[1].split("<")[0]
            documented_intrinsics.add(parse_fn(intrinsics, True))

undocumented = gcc_intrinsics - documented_intrinsics
for entry in undocumented:
    print("Undocumented:", " ".join(entry))
    for e in documented_intrinsics:
        if e[1] == entry[1]:
            print("Matching:", " ".join(e))