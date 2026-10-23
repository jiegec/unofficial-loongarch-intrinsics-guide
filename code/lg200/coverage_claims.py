#!/usr/bin/env python3
"""A clause must not claim it is untested while naming a value its own cases carry.

Several rows in this table said 'no case covers it' or 'the checked values are small' about a
hex value that the row's own vectors already exercised.  Each was a question already answered,
left standing because the prose and the case grid were never compared.

This gate does exactly that comparison: an UNRESOLVED sentence that makes a coverage claim and
also names a hex value present in the row's own case inputs is a self-contradiction.
"""
import csv, pathlib, re, sys
REPO = pathlib.Path(__file__).resolve().parent.parent.parent
CLAIM = ['no case', 'no verified vector', 'not been exercised', 'untested', 'not covered',
         'no vector', 'are small', 'is untested', 'no experiment', 'has not been', 'no cases']
HEX = re.compile(r'0x[0-9a-fA-F]{4,8}')
def main():
    carried = {}
    for line in (REPO/'code/lg200/generated_cases.tsv').read_text().splitlines():
        p = line.split(chr(9))
        if len(p) < 4: continue
        carried.setdefault(p[0], set()).update(int(w, 16) for w in p[2].split() if w.startswith('0x'))
    bad = []
    for row in csv.DictReader(open(REPO/'code/lg200-instructions.tsv'), delimiter=chr(9)):
        m = row['mnemonic']
        vals = carried.get(m, set())
        if not vals: continue
        text = row['description'].replace(chr(92) + 'n', ' ')
        for s in re.split(r'(?<=[.])\s+', text):
            if 'UNRESOLVED' not in s: continue
            if not any(c in s.lower() for c in CLAIM): continue
            named = {int(h, 16) for h in HEX.findall(s)}
            shared = named & vals
            if shared: bad.append((m, sorted(shared)[:2], s[:100]))
    for m, v, s in bad:
        print('CLAIM NAMES A CARRIED VALUE %-20s %s %s' % (m, [hex(x) for x in v], s[:80]))
    print('coverage claims contradicted by their own cases: %d' % len(bad))
    return 1 if bad else 0
if __name__ == '__main__':
    sys.exit(main())