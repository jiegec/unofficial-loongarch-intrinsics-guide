uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c;
if (n != 0) {
  unsigned carry_out = ((v >> (32 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint32_t r = (uint32_t)(v << 1);
    EFLAGS.OF = ((r & 0x80000000) != 0) != carry_out;
  }
}
