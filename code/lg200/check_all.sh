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
# The mutation tool rewrites tracked files for the length of each case, so a suite run beside it
# reads deliberately broken files and reports them as failures.  That is not hypothetical: the
# two were started together and the suite failed at step one.  The lock is the fix.
# The mutation tool holds the lock for its whole run, and one of its cases runs this
# suite as its check - so that case would only ever see the refusal, which is exactly
# what happened: it reported fault exit=1 and proved nothing from then on.
if [ -e /tmp/lg200-mutation.lock ] && [ -z "$LG200_MUTATION_NESTED" ]; then
  echo "a mutation run holds /tmp/lg200-mutation.lock; refusing to read mutated files"
  exit 1
fi

HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)

cd "$HERE"
echo "== set_desc --check"
python3 set_desc.py --check
echo "== header_sync --check"
python3 header_sync.py --check
# The oracle binary is not rebuilt by run_cases.py, so a case file that stops compiling
# leaves the previous binary in place and every CPU case keeps passing against a model that
# no longer matches.  That is exactly what had happened: 139 case files had a space where the
# macro wanted a comma, the build had been failing, and this suite stayed green.  Building
# first makes a broken case file fail here instead of hiding behind a stale binary.
echo "== run_cases"
make -s build/lgpu-cases && python3 run_cases.py | tail -1
echo "== prose_audit"
python3 prose_audit.py | head -1
echo "== doc_register_audit"
python3 doc_register_audit.py | sed -n '2p'
echo "== format_audit"
python3 format_audit.py | tail -1
echo "== case_coverage"
python3 case_coverage.py | head -1
echo "== doc_gate"
cd "$REPO" && python3 code/lg200/doc_gate.py --all | tail -1
echo "== md_table_check"
cd "$REPO" && python3 code/lg200/md_table_check.py | tail -1
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
echo "== addr_scan"
cd "$REPO" && python3 code/lg200/gpu/addr_scan.py --selftest | tail -1
cd "$REPO" && python3 code/lg200/gpu/addr_scan.py | tail -1
echo "== coverage_claims"
cd "$REPO" && python3 code/lg200/coverage_claims.py | tail -1
echo "== observed_reasons"
cd "$REPO" && python3 code/lg200/observed_reasons.py | tail -1
# The observe-mode readback (build/gpu_obs/) is a device-produced artifact, so
# this is a note until a non-dry run has written one and a gate afterwards: a
# record that is present but older than the matrix, or not what the matrix says,
# is a claim about hardware that the tree no longer supports.  Both checks run
# under the one step name above, so the suite's own accounting keeps one entry
# per gate.
cd "$REPO" && python3 code/lg200/observed_reasons.py --samples | tail -4
echo "== check_gpu"
# check_gpu.py lives at the checkout root, not beside these scripts.
cd "$REPO"
python3 check_gpu.py | tail -1
echo "ALL CHECKS PASSED"
