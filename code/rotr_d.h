uint64_t v = (uint64_t)a;
unsigned c = (unsigned)b % 64;
uint64_t r = c == 0 ? v : (uint64_t)((v >> c) | (v << (64 - c)));
dst = r;