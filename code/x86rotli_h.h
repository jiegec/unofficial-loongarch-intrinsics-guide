uint16_t v = (uint16_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 16;
uint8_t carry_out = n == 0 ? ((v >> (16 - 1)) & 1) : ((v >> (16 - n)) & 1);
uint16_t r = n == 0 ? v : (uint16_t)((v << n) | (v >> (16 - n)));
EFLAGS.CF = carry_out;
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000) != 0;
}
