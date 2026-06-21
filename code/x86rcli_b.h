uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(imm & 0x1f);
unsigned n = c % 9;
if (n != 0) {
  unsigned carry_out = ((v >> (8 - n)) & 1);
  EFLAGS.CF = carry_out;
  if (c == 1) {
    uint8_t r = (uint8_t)(v << 1);
    EFLAGS.OF = ((r & 0x80) != 0) != carry_out;
  }
}
