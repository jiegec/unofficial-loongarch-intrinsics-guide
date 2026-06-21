uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 17;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (16 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint16_t r = (uint16_t)((v >> 1) | (carry_in << 15));
    EFLAGS.OF = (((r >> 15) ^ (r >> 14)) & 1) != 0;
  }
}
