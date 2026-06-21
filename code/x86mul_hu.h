uint16_t lhs = (uint16_t)a;
uint16_t rhs = (uint16_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint16_t)lhs * (unsigned __int128)(uint16_t)rhs;
bool overflow = (product >> 16) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;