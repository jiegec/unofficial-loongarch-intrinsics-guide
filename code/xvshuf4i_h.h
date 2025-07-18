for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.half[0] = a.half[0 + ((imm >> 0) & 0x3)];
  dst.half[1] = a.half[0 + ((imm >> 2) & 0x3)];
  dst.half[2] = a.half[0 + ((imm >> 4) & 0x3)];
  dst.half[3] = a.half[0 + ((imm >> 6) & 0x3)];
  dst.half[4] = a.half[4 + ((imm >> 0) & 0x3)];
  dst.half[5] = a.half[4 + ((imm >> 2) & 0x3)];
  dst.half[6] = a.half[4 + ((imm >> 4) & 0x3)];
  dst.half[7] = a.half[4 + ((imm >> 6) & 0x3)];
  dst.half[8] = a.half[8 + ((imm >> 0) & 0x3)];
  dst.half[9] = a.half[8 + ((imm >> 2) & 0x3)];
  dst.half[10] = a.half[8 + ((imm >> 4) & 0x3)];
  dst.half[11] = a.half[8 + ((imm >> 6) & 0x3)];
  dst.half[12] = a.half[12 + ((imm >> 0) & 0x3)];
  dst.half[13] = a.half[12 + ((imm >> 2) & 0x3)];
  dst.half[14] = a.half[12 + ((imm >> 4) & 0x3)];
  dst.half[15] = a.half[12 + ((imm >> 6) & 0x3)];
}
