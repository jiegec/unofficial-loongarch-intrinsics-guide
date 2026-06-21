uint32_t v = (uint32_t)a;
unsigned c = (unsigned)b % 32;
uint32_t r = c == 0 ? v : (uint32_t)((v >> c) | (v << (32 - c)));
dst = sext(r, 32);