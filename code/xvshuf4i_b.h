for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.byte[0] = a.byte[0 + ((imm >> 0) & 0x3)];
  dst.byte[1] = a.byte[0 + ((imm >> 2) & 0x3)];
  dst.byte[2] = a.byte[0 + ((imm >> 4) & 0x3)];
  dst.byte[3] = a.byte[0 + ((imm >> 6) & 0x3)];
  dst.byte[4] = a.byte[4 + ((imm >> 0) & 0x3)];
  dst.byte[5] = a.byte[4 + ((imm >> 2) & 0x3)];
  dst.byte[6] = a.byte[4 + ((imm >> 4) & 0x3)];
  dst.byte[7] = a.byte[4 + ((imm >> 6) & 0x3)];
  dst.byte[8] = a.byte[8 + ((imm >> 0) & 0x3)];
  dst.byte[9] = a.byte[8 + ((imm >> 2) & 0x3)];
  dst.byte[10] = a.byte[8 + ((imm >> 4) & 0x3)];
  dst.byte[11] = a.byte[8 + ((imm >> 6) & 0x3)];
  dst.byte[12] = a.byte[12 + ((imm >> 0) & 0x3)];
  dst.byte[13] = a.byte[12 + ((imm >> 2) & 0x3)];
  dst.byte[14] = a.byte[12 + ((imm >> 4) & 0x3)];
  dst.byte[15] = a.byte[12 + ((imm >> 6) & 0x3)];
  dst.byte[16] = a.byte[16 + ((imm >> 0) & 0x3)];
  dst.byte[17] = a.byte[16 + ((imm >> 2) & 0x3)];
  dst.byte[18] = a.byte[16 + ((imm >> 4) & 0x3)];
  dst.byte[19] = a.byte[16 + ((imm >> 6) & 0x3)];
  dst.byte[20] = a.byte[20 + ((imm >> 0) & 0x3)];
  dst.byte[21] = a.byte[20 + ((imm >> 2) & 0x3)];
  dst.byte[22] = a.byte[20 + ((imm >> 4) & 0x3)];
  dst.byte[23] = a.byte[20 + ((imm >> 6) & 0x3)];
  dst.byte[24] = a.byte[24 + ((imm >> 0) & 0x3)];
  dst.byte[25] = a.byte[24 + ((imm >> 2) & 0x3)];
  dst.byte[26] = a.byte[24 + ((imm >> 4) & 0x3)];
  dst.byte[27] = a.byte[24 + ((imm >> 6) & 0x3)];
  dst.byte[28] = a.byte[28 + ((imm >> 0) & 0x3)];
  dst.byte[29] = a.byte[28 + ((imm >> 2) & 0x3)];
  dst.byte[30] = a.byte[28 + ((imm >> 4) & 0x3)];
  dst.byte[31] = a.byte[28 + ((imm >> 6) & 0x3)];
}
