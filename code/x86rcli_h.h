uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 17;
if (n != 0) {
  unsigned carry_out = ((v >> (16 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint16_t r = (uint16_t)(v << 1);
    EFLAGS.OF = ((r & 0x8000) != 0) != carry_out;
  }
}
