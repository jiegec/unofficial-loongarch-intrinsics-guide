uint32_t v = (uint32_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 32;
uint32_t r = (uint32_t)((v >> n) | (v << (32 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 32) % 32)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x80000000) != 0);
}
