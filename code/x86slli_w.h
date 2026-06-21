uint32_t v = (uint32_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 32 ? 0 : ((v >> (32 - c)) & 1);
  uint32_t r = c >= 32 ? 0 : (uint32_t)(v << c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int32_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
  }
}
