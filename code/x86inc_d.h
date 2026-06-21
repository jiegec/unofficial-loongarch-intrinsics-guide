uint64_t v = (uint64_t)a;
uint64_t r = v + 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int64_t)r < 0;
EFLAGS.OF = v == (uint64_t)INT64_MIN;