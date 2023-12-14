import glob
import os

for f in sorted(glob.glob("*.h")):
    src = f.replace(".h", ".cpp")
    if os.path.exists(src):
        # make sure we are testing the expected instruction
        content = open(src, "r").read()
        name = src.split(".")[0]
        if name not in content or f'#include "{name}.h"' not in content:
            print("Bad test:", src)
    if not os.path.exists(src):
        if (
            src.startswith("xvf")
            or src.startswith("vf")
            or src.startswith("xvld")
            or src.startswith("vld")
            or src.startswith("xvst")
            or src.startswith("vst")
            or src == "common.cpp"
        ):
            # skipped
            continue
        print(f"Missing {src}")
