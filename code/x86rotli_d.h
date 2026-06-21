uint64_t v = (uint64_t)a;
unsigned c = (unsigned)imm;
unsigned n = c % 64;
uint64_t r = (uint64_t)((v << n) | (v >> (64 - n)));
EFLAGS.CF = ((v >> ((64 - n) % 64)) & 1);
if (c == 1) {
  EFLAGS.OF = ((v ^ r) & 0x8000000000000000ULL) != 0;
}
