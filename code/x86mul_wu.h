uint32_t lhs = (uint32_t)a;
uint32_t rhs = (uint32_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint32_t)lhs * (unsigned __int128)(uint32_t)rhs;
bool overflow = (product >> 32) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
