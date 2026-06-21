uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
uint16_t result = lhs | rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int16_t)result < 0;
