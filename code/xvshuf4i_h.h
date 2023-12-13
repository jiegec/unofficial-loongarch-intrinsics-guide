for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}
