uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
uint8_t result = lhs - rhs;
EFLAGS.CF = lhs < rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((lhs ^ rhs) & (lhs ^ result) & 0x80) != 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int8_t)result < 0;
