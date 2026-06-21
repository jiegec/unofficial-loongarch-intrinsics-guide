uint8_t v = (uint8_t)a;
unsigned c = imm % 8;
uint8_t r = c == 0 ? v : (uint8_t)((v >> c) | (v << (8 - c)));
dst = sext(r, 8);