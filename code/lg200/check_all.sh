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
set -e
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
cd "$REPO"
echo "== check_gpu"
python3 check_gpu.py | tail -1
echo "ALL CHECKS PASSED"
