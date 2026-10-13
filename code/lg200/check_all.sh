#!/bin/sh
# Every mechanical check this repository has, in the order that fails fastest.
#
# Run from anywhere; paths are resolved from this script.  Exits non-zero on the first
# failure.  The set grew one checker at a time as this campaign found new ways for the
# documentation to drift away from the rows it describes:
#
#   set_desc.py --check      the tsv's own shape (768 rows, 23 columns, unique mnemonics)
#   header_sync.py --check   each header's first comment line against its row
#   run_cases.py             the offline oracle: every instruction's case vectors
#   check_gpu.py             the repository's wider invariants
#   check_doc_numbers.py     the numbers quoted in the docs against the sources
#   prose_audit.py           a row's prose against its own layout
#   format_audit.py          the encoding table's field lists against the rows
#
# None of these touch hardware, so they are safe to run at any time.
#
# pipefail is not decoration.  With plain `set -e`, a check written as `python3 x.py |
# tail -1` reports success when x.py fails, because the pipeline's status is tail's.  That
# went unnoticed on 2026-10-13 when this script ran a check whose path I had just broken:
# the shell printed "can't open file" and the suite still printed ALL CHECKS PASSED.  A
# suite that reports success while a check is missing is worse than no suite.
set -eo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)

cd "$HERE"
echo "== set_desc --check"
python3 set_desc.py --check
echo "== header_sync --check"
python3 header_sync.py --check
echo "== run_cases"
python3 run_cases.py | tail -1
echo "== check_doc_numbers"
python3 check_doc_numbers.py | tail -1
echo "== prose_audit"
python3 prose_audit.py | head -1
echo "== doc_register_audit"
python3 doc_register_audit.py | sed -n '2p'
echo "== format_audit"
python3 format_audit.py | tail -1
echo "== rule_dupe_check"
python3 rule_dupe_check.py
echo "== gen_state_map"
python3 gen_state_map.py --check | tail -1
echo "== bench_doc_check"
python3 bench_doc_check.py
echo "== md_table_check"
cd "$REPO" && python3 code/lg200/md_table_check.py | tail -1
echo "== fp_mode_audit"
cd "$REPO" && python3 code/lg200/fp_mode_audit.py --check | tail -1
echo "== case_provenance"
cd "$REPO" && python3 code/lg200/case_provenance.py --check | tail -1
echo "== gate_check"
# Field coverage needs a runner listing, which is a device-produced artifact and is
# not tracked.  With one present this is a gate; without one there is nothing to audit,
# and saying so is better than passing quietly.
# Both cross-check this tree against the closed driver and the vendor assembler,
# so they need the LCO checkout present.  With it they are gates; without it there
# is nothing to compare against, and the step says so.
echo "== jumpstack_check"
if [ -d /home/lin/work/src/loonggpu ]; then
  cd "$REPO" && python3 code/lg200/gpu/jumpstack_check.py | tail -1
else
  echo "no LCO checkout; the jump-stack cross-check needs the vendor ISA row"
fi
echo "== enc_probe"
if [ -d /home/lin/work/src/loonggpu ]; then
  cd "$REPO" && python3 code/lg200/gpu/enc_probe.py | tail -1
else
  echo "no LCO checkout; the encoder cross-check needs the vendor assembler"
fi
echo "== audit"
if ls "$REPO"/code/lg200/gpu/build/gpu-run-logs/*.log >/dev/null 2>&1; then
  cd "$REPO" && python3 code/lg200/gpu/audit.py | tail -1
else
  echo "no runner listing present; the field-coverage audit runs after a session"
fi
cd "$REPO" && python3 code/lg200/gate_check.py | tail -1
echo "== addr_scan"
cd "$REPO" && python3 code/lg200/gpu/addr_scan.py --selftest | tail -1
cd "$REPO" && python3 code/lg200/gpu/addr_scan.py | tail -1
echo "== check_gpu"
# check_gpu.py lives at the checkout root, not beside these scripts.
cd "$REPO"
python3 check_gpu.py | tail -1
echo "ALL CHECKS PASSED"
