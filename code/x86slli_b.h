uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 8 ? 0 : ((v >> (8 - c)) & 1);
  uint8_t r = c >= 8 ? 0 : (uint8_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int8_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80) != 0;
  }
}
