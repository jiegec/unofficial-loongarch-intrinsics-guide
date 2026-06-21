uint64_t lhs = (uint64_t)a;
uint64_t rhs = (uint64_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint64_t)lhs * (unsigned __int128)(uint64_t)rhs;
bool overflow = (product >> 64) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;