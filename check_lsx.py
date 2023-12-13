import os
import glob

# Initial worktree:
# git worktree add -f ../gh-pages gh-pages
# Update gh-pages before running:
# git fetch origin gh-pages && git -C ../gh-pages reset origin/gh-pages --hard

# gcc intrinsics
gcc_intrinsics = set()
for line in open('gcc_lsxintrin.h', 'r'):
    gcc_intrinsics.add(tuple(line.strip()[:-1].split(' ')))
#print(gcc_intrinsics)

# find documented intrinsics
documented_intrinsics = set()
for f in glob.glob("../gh-pages/**/*.html", recursive=True):
    # forgive me to use the simple but fragile way to parse html
    for line in open(f, 'r'):
        if "h2" in line:
            intrinsics = line.split(">")[1].split("<")[0]

            # strip off names that gcc does not have
            parts = intrinsics.split(" ")
            begin_params = False
            i = 0
            while i < len(parts):
                if "(" in parts[i]:
                    begin_params = True
                    i += 1
                elif "," in parts[i]:
                    parts = parts[:i] + parts[i+1:]
                elif ")" in parts[i]:
                    parts = parts[:i] + parts[i+1:]
                    parts[i-1] += ")"
                    i += 1
                elif begin_params:
                    parts[i-1] += ","
                    i += 1
                else:
                    i += 1
            documented_intrinsics.add(tuple(parts))

undocumented = gcc_intrinsics - documented_intrinsics
for entry in undocumented:
    print("Undocumented:", " ".join(entry))