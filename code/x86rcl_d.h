uint64_t v = (uint64_t)a;
unsigned c = (unsigned)(b & 0x3f);
unsigned n = c;
if (n != 0) {
  unsigned carry_out = ((v >> (64 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint64_t r = (uint64_t)(v << 1);
    EFLAGS.OF = ((r & 0x8000000000000000ULL) != 0) != carry_out;
  }
}
