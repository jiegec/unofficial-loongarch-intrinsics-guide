uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 64;
uint8_t carry_out = n == 0 ? ((v >> (64 - 1)) & 1) : ((v >> (64 - n)) & 1);
uint64_t r = n == 0 ? v : (uint64_t)((v << n) | (v >> (64 - n)));
EFLAGS.CF = carry_out;
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
}
