# Vendored MCS builder

`loongvk_mcs.c` / `loongvk_mcs.h` are copied from the LoongVK driver in the
loonggpu tree:

    src/mesa/src/loonggpu/vulkan/loongvk_mcs.{c,h}

They are MIT-licensed (LoongGPU contributors), as is this project, so vendoring
is a licence-compatible copy rather than a rewrite. The point of copying rather
than re-deriving is that this builder has already been checked against the
vendor's own output; re-typing it would throw that away.

## What changed

**One line.** `#include "loongvk_kd.h"` became `#include "mcs_kd.h"`.

`mcs_kd.h` carries the five things the builder actually uses from LoongVK's
kernel-descriptor header (`LOONGVK_KD_RSRC1`, the two SGPR counts, the float-mode
constants and `loongvk_kd_granulated`). Vendoring the original header instead
would pull in `lco_abi.h` and with it a Mesa build dependency, which this project
must not have. The values are copied verbatim and say so.

Everything else is untouched.

## Verification

    gcc -std=gnu11 -O2 -Wall -Wextra -o verify verify_against_vendor.c loongvk_mcs.c
    ./verify
    # len=192 bytes, 0 differing

`verify_against_vendor.c` builds one dispatch with this builder and compares it
against the bytes produced by the **vendor's own emitters**, obtained from the
loonggpu tree's `analysis/lacm/mcs_ref.c`. That harness `dlopen`s
`libvdd-runtime64.so.0.1.0`, calls `AppendStscfgCommand`, the group/wait/cinv/
sbmt/wbdbar emitters and the NOP padder *by address*, and prints the result — so
the expected array is the vendor's ground truth for a real `add.cl` dispatch, not
a reading of it.

The inputs are the live-captured values from that dispatch. Two are recovered
from the expected bytes rather than captured:

* `rsrc0 = 0x42` needs `granulated(sgprs, vgprs) = 0x42`, i.e. `sgprs` in 1..8 and
  `vgprs` in 5..8; 8 and 8 are used.
* `rsrc1' = 0x083c2076` is what `shape_rsrc1(LOONGVK_KD_RSRC1, {1,1,1},
  FLOAT_MODE_DEFAULT)` returns — `0x08000000 | 0x2076 | 0x3c0000`.

One field is an **input**, not something the builder derives, and leaving it zero
is what produced the only difference in the first run:

    scratch_resource_descriptor = { 0, 0x80000000, 0, 0x00ca4fac }

That four-dword private-buffer descriptor is what the vendor hardcodes for a
dispatch with no scratch region. Worth knowing before wiring this up: a carrier
that allocates no private segment still has to send this word, and the low half
of its last dword carries the `.kd` flag bits.

## What this does and does not prove

It proves the **stream encoding** is byte-exact against the vendor. It says
nothing about whether the firmware accepts a stream from our ring — that is what
the carrier's first hardware run has to establish, and the acceptance test for it
has to be a case already known to pass through LoongVK's path.
