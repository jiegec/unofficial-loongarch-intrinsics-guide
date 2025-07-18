for (int i = 0; i < 16; i++) {
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
  dst.half[8] = a.half[(8 & (~0x3)) + ((imm >> (2 * (8 & 0x3))) & 0x3)];
  dst.half[9] = a.half[(9 & (~0x3)) + ((imm >> (2 * (9 & 0x3))) & 0x3)];
  dst.half[10] = a.half[(10 & (~0x3)) + ((imm >> (2 * (10 & 0x3))) & 0x3)];
  dst.half[11] = a.half[(11 & (~0x3)) + ((imm >> (2 * (11 & 0x3))) & 0x3)];
  dst.half[12] = a.half[(12 & (~0x3)) + ((imm >> (2 * (12 & 0x3))) & 0x3)];
  dst.half[13] = a.half[(13 & (~0x3)) + ((imm >> (2 * (13 & 0x3))) & 0x3)];
  dst.half[14] = a.half[(14 & (~0x3)) + ((imm >> (2 * (14 & 0x3))) & 0x3)];
  dst.half[15] = a.half[(15 & (~0x3)) + ((imm >> (2 * (15 & 0x3))) & 0x3)];
}
