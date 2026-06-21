uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
if (c != 0) {
  uint8_t carry_out = c > 64 ? 0 : ((v >> (c - 1)) & 1);
  uint64_t r = c >= 64 ? 0 : (v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int64_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
  }
}
