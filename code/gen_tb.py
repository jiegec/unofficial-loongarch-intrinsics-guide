widths_all = ["b", "bu", "h", "hu", "w", "wu", "d", "du"]

tb = {
    "vavg": (widths_all, "v128 a, v128 b"),
    "vavgr": (widths_all, "v128 a, v128 b")
}

for name in tb:
    t = tb[name]
    widths = t[0]
    args = t[1]

    for width in widths:
        inst_name = name + "_" + width

        fuzz_args = 0
        for arg in args.split(', '):
            if 'v128' in arg:
                fuzz_args += 1

        print(f'Saving {inst_name}.cpp')
        with open(f'{inst_name}.cpp', 'w') as f:
            print('#include "common.h"', file=f)
            print('', file=f)
            print(f'v128 {inst_name}({args}) {{', file=f)
            print('  v128 dst;', file=f)
            print(f'#include "{inst_name}.h"', file=f)
            print('  return dst;', file=f)
            print('}', file=f)
            print('', file=f)
            print('void test() {', file=f)
            print(f'  FUZZ{fuzz_args}({inst_name});', file=f)
            print('}', file=f)