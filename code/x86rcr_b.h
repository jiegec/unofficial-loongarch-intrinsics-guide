uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 9;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (8 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint8_t r = (uint8_t)((v >> 1) | (carry_in << 7));
    EFLAGS.OF = (((r >> 7) ^ (r >> 6)) & 1) != 0;
  }
}
