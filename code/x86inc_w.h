uint32_t v = (uint32_t)a;
uint32_t r = v + 1;
// CF preserved from input EFLAGS.CF
EFLAGS.PF = parity_even((uint8_t)r);
EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;
EFLAGS.ZF = r == 0;
EFLAGS.SF = (int32_t)r < 0;
EFLAGS.OF = v == (uint32_t)INT32_MIN;