uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
unsigned n = c;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (64 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint64_t r = (uint64_t)((v >> 1) | ((uint64_t)carry_in << 63));
    EFLAGS.OF = (((r >> 63) ^ (r >> 62)) & 1) != 0;
  }
}
