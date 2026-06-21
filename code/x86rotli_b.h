uint8_t v = (uint8_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 8;
uint8_t carry_out = n == 0 ? ((v >> (8 - 1)) & 1) : ((v >> (8 - n)) & 1);
uint8_t r = n == 0 ? v : (uint8_t)((v << n) | (v >> (8 - n)));
EFLAGS.CF = carry_out;
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x80) != 0;
}
