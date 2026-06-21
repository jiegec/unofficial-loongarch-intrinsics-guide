uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c;
if (n != 0) {
  unsigned carry_in = EFLAGS.CF;
  unsigned carry_out = ((v >> ((n - 1) % (32 + 1))) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint32_t r = (uint32_t)((v >> 1) | (carry_in << 31));
    EFLAGS.OF = (((r >> 31) ^ (r >> 30)) & 1) != 0;
  }
}
