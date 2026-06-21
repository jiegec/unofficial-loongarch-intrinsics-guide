uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
uint64_t result = lhs + rhs;
EFLAGS.CF = lhs > UINT64_MAX - rhs;
EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;
EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & 0x8000000000000000ULL) != 0;
EFLAGS.PF = parity_even((uint8_t)result);
EFLAGS.ZF = result == 0;
EFLAGS.SF = (int64_t)result < 0;
