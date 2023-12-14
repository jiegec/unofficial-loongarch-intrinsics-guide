import glob
import os

for f in sorted(glob.glob("*.h")):
    src = f.replace(".h", ".cpp")
    if not os.path.exists(src):
        if (
            src.startswith("xvf")
            or src.startswith("vf")
            or src.startswith("xvset")
            or src.startswith("vset")
            or src.startswith("xvld")
            or src.startswith("vld")
            or src.startswith("xvst")
            or src.startswith("vst")
        ):
            # skipped
            continue
        print(f"Missing {src}")
