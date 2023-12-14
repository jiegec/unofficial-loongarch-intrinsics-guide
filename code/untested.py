import glob
import os

for f in sorted(glob.glob("*.h")):
    src = f.replace(".h", ".cpp")
    if not os.path.exists(src):
        print(f"Missing {src}")