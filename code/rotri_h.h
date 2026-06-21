uint16_t v = (uint16_t)a;
unsigned c = imm % 16;
uint16_t r = c == 0 ? v : (uint16_t)((v >> c) | (v << (16 - c)));
dst = sext(r, 16);