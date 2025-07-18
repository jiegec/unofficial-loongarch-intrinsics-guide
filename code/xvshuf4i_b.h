for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.byte[0] = a.byte[(0 & (~0x3)) + ((imm >> (2 * (0 & 0x3))) & 0x3)];
  dst.byte[1] = a.byte[(1 & (~0x3)) + ((imm >> (2 * (1 & 0x3))) & 0x3)];
  dst.byte[2] = a.byte[(2 & (~0x3)) + ((imm >> (2 * (2 & 0x3))) & 0x3)];
  dst.byte[3] = a.byte[(3 & (~0x3)) + ((imm >> (2 * (3 & 0x3))) & 0x3)];
  dst.byte[4] = a.byte[(4 & (~0x3)) + ((imm >> (2 * (4 & 0x3))) & 0x3)];
  dst.byte[5] = a.byte[(5 & (~0x3)) + ((imm >> (2 * (5 & 0x3))) & 0x3)];
  dst.byte[6] = a.byte[(6 & (~0x3)) + ((imm >> (2 * (6 & 0x3))) & 0x3)];
  dst.byte[7] = a.byte[(7 & (~0x3)) + ((imm >> (2 * (7 & 0x3))) & 0x3)];
  dst.byte[8] = a.byte[(8 & (~0x3)) + ((imm >> (2 * (8 & 0x3))) & 0x3)];
  dst.byte[9] = a.byte[(9 & (~0x3)) + ((imm >> (2 * (9 & 0x3))) & 0x3)];
  dst.byte[10] = a.byte[(10 & (~0x3)) + ((imm >> (2 * (10 & 0x3))) & 0x3)];
  dst.byte[11] = a.byte[(11 & (~0x3)) + ((imm >> (2 * (11 & 0x3))) & 0x3)];
  dst.byte[12] = a.byte[(12 & (~0x3)) + ((imm >> (2 * (12 & 0x3))) & 0x3)];
  dst.byte[13] = a.byte[(13 & (~0x3)) + ((imm >> (2 * (13 & 0x3))) & 0x3)];
  dst.byte[14] = a.byte[(14 & (~0x3)) + ((imm >> (2 * (14 & 0x3))) & 0x3)];
  dst.byte[15] = a.byte[(15 & (~0x3)) + ((imm >> (2 * (15 & 0x3))) & 0x3)];
  dst.byte[16] = a.byte[(16 & (~0x3)) + ((imm >> (2 * (16 & 0x3))) & 0x3)];
  dst.byte[17] = a.byte[(17 & (~0x3)) + ((imm >> (2 * (17 & 0x3))) & 0x3)];
  dst.byte[18] = a.byte[(18 & (~0x3)) + ((imm >> (2 * (18 & 0x3))) & 0x3)];
  dst.byte[19] = a.byte[(19 & (~0x3)) + ((imm >> (2 * (19 & 0x3))) & 0x3)];
  dst.byte[20] = a.byte[(20 & (~0x3)) + ((imm >> (2 * (20 & 0x3))) & 0x3)];
  dst.byte[21] = a.byte[(21 & (~0x3)) + ((imm >> (2 * (21 & 0x3))) & 0x3)];
  dst.byte[22] = a.byte[(22 & (~0x3)) + ((imm >> (2 * (22 & 0x3))) & 0x3)];
  dst.byte[23] = a.byte[(23 & (~0x3)) + ((imm >> (2 * (23 & 0x3))) & 0x3)];
  dst.byte[24] = a.byte[(24 & (~0x3)) + ((imm >> (2 * (24 & 0x3))) & 0x3)];
  dst.byte[25] = a.byte[(25 & (~0x3)) + ((imm >> (2 * (25 & 0x3))) & 0x3)];
  dst.byte[26] = a.byte[(26 & (~0x3)) + ((imm >> (2 * (26 & 0x3))) & 0x3)];
  dst.byte[27] = a.byte[(27 & (~0x3)) + ((imm >> (2 * (27 & 0x3))) & 0x3)];
  dst.byte[28] = a.byte[(28 & (~0x3)) + ((imm >> (2 * (28 & 0x3))) & 0x3)];
  dst.byte[29] = a.byte[(29 & (~0x3)) + ((imm >> (2 * (29 & 0x3))) & 0x3)];
  dst.byte[30] = a.byte[(30 & (~0x3)) + ((imm >> (2 * (30 & 0x3))) & 0x3)];
  dst.byte[31] = a.byte[(31 & (~0x3)) + ((imm >> (2 * (31 & 0x3))) & 0x3)];
}
