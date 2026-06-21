uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c;
uint8_t carry_out = n == 0 ? ((v >> (32 - 1)) & 1) : ((v >> (32 - n)) & 1);
uint32_t r = n == 0 ? v : (uint32_t)((v << n) | (v >> (32 - n)));
EFLAGS.CF = carry_out;
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x80000000) != 0;
}
