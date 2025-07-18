for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.half[0] = a.half[(0 & (~0x3)) + ((imm >> (2 * (0 & 0x3))) & 0x3)];
  dst.half[1] = a.half[(1 & (~0x3)) + ((imm >> (2 * (1 & 0x3))) & 0x3)];
  dst.half[2] = a.half[(2 & (~0x3)) + ((imm >> (2 * (2 & 0x3))) & 0x3)];
  dst.half[3] = a.half[(3 & (~0x3)) + ((imm >> (2 * (3 & 0x3))) & 0x3)];
  dst.half[4] = a.half[(4 & (~0x3)) + ((imm >> (2 * (4 & 0x3))) & 0x3)];
  dst.half[5] = a.half[(5 & (~0x3)) + ((imm >> (2 * (5 & 0x3))) & 0x3)];
  dst.half[6] = a.half[(6 & (~0x3)) + ((imm >> (2 * (6 & 0x3))) & 0x3)];
  dst.half[7] = a.half[(7 & (~0x3)) + ((imm >> (2 * (7 & 0x3))) & 0x3)];
}
