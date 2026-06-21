uint8_t v = (uint8_t)a;
unsigned c = (unsigned)(b & 0x1f);
unsigned n = c % 8;
uint8_t r = (uint8_t)((v >> n) | (v << (8 - n)));
EFLAGS.CF = ((v >> ((n - 1 + 8) % 8)) & 1);
if (c == 1) {
  EFLAGS.OF = (((v ^ r) & 0x80) != 0);
}
