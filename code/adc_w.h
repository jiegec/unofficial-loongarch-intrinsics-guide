uint32_t lhs = a;
uint32_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint32_t r = lhs + rhs + cf;
dst = sext(r, 32);
