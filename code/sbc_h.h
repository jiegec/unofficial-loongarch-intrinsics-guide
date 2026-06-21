uint16_t lhs = a;
uint16_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint16_t r = lhs - rhs - cf;
dst = sext(r, 16);
