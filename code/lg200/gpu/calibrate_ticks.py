#!/usr/bin/env python3
"""Calibrate the GPU timestamp tick against the host clock.

The CS completion-signal BO carries four 64-bit firmware counters at
+0x20/+0x28/+0x30/+0x38 (enqueue, retire, wave start, wave end).  The counter is
free-running, so two single submissions separated by a known host sleep give its
rate directly:

    ns_per_tick = host_seconds * 1e9 / (ts20_b - ts20_a)

Run it with the device otherwise idle.  The sleep is generous (default 30 s) so
the host-side jitter of two process launches is well under 1%.

    python3 calibrate_ticks.py [sleep_seconds]
"""
import re
import subprocess
import sys
import time

BIN = "./build/lgpu-gpu"


def sample():
    out = subprocess.run([BIN, "cspersist", "1"], capture_output=True,
                         text=True, timeout=120).stdout
    m = re.search(r"ts20=0x([0-9a-f]+)", out)
    if not m:
        raise SystemExit("no ts20 in: " + out)
    return int(m.group(1), 16)


def main():
    secs = float(sys.argv[1]) if len(sys.argv) > 1 else 30.0
    a = sample()
    t0 = time.monotonic()
    time.sleep(secs)
    b = sample()
    dt = time.monotonic() - t0
    d = b - a
    if d <= 0:
        raise SystemExit("counter did not advance")
    rate = d / dt
    print("tick delta    %d" % d)
    print("host seconds  %.3f" % dt)
    print("ticks/second  %.6g" % rate)
    print("ns/tick       %.4f" % (dt * 1e9 / d))


if __name__ == "__main__":
    main()
