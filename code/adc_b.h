uint8_t lhs = a;
uint8_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint8_t r = lhs + rhs + cf;
dst = sext(r, 8);
