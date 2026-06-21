uint8_t lhs = (uint8_t)a;
uint8_t rhs = (uint8_t)b;
unsigned __int128 product =
    (unsigned __int128)(uint8_t)lhs * (unsigned __int128)(uint8_t)rhs;
bool overflow = (product >> 8) != 0;
EFLAGS.CF = overflow;
EFLAGS.OF = overflow;
EFLAGS.SF = 0;
EFLAGS.ZF = 0;
EFLAGS.AF = 0;
EFLAGS.PF = 0;
