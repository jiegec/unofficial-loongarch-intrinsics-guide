for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.word[0] = a.word[0 + ((imm >> 0) & 0x3)];
  dst.word[1] = a.word[0 + ((imm >> 2) & 0x3)];
  dst.word[2] = a.word[0 + ((imm >> 4) & 0x3)];
  dst.word[3] = a.word[0 + ((imm >> 6) & 0x3)];
}
