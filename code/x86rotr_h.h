uint16_t v = (uint16_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 16;
uint16_t r = (uint16_t)((v >> n) | (v << (16 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 16) % 16)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x8000) != 0);
}
