for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}
