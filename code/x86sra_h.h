uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
if (c != 0) {
  uint8_t carry_out = c > 16 ? 0 : ((v >> (c - 1)) & 1);
  uint16_t r = c >= 16 ? (uint16_t)((int16_t)v >> (16 - 1))
                       : (uint16_t)((int16_t)v >> c);
  EFLAGS.CF = carry_out;
  EFLAGS.PF = parity_even((uint8_t)r);
  EFLAGS.ZF = r == 0;
  EFLAGS.SF = (int16_t)r < 0;
  if (c == 1) {
    EFLAGS.OF = 0;
  }
}
