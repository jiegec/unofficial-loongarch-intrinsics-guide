uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
uint32_t result = lhs & rhs;
EFLAGS.CF = 0;
EFLAGS.OF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int32_t)result < 0;