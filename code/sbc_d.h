uint64_t lhs = a;
uint64_t rhs = b;
uint8_t cf = EFLAGS.CF;
uint64_t r = lhs - rhs - cf;
dst = r;
