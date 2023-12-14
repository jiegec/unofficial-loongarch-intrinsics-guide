import glob
import os

for f in sorted(glob.glob("*.h")):
    src = f.replace(".h", ".cpp")
    if os.path.exists(src):
        # make sure we are testing the expected instruction
        if src.split(".")[0] not in open(src, 'r').read():
            # intrinsic's name is different from instruction's name
            if src.startswith("vset") or src.startswith("vset"):
                continue
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
