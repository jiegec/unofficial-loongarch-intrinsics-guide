for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i == ((imm >> 4) & 15)) ? b.byte[imm & 15] : a.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = (0 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[0]);
  dst.byte[1] = (1 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[1]);
  dst.byte[2] = (2 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[2]);
  dst.byte[3] = (3 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[3]);
  dst.byte[4] = (4 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[4]);
  dst.byte[5] = (5 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[5]);
  dst.byte[6] = (6 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[6]);
  dst.byte[7] = (7 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[7]);
  dst.byte[8] = (8 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[8]);
  dst.byte[9] = (9 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[9]);
  dst.byte[10] = (10 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[10]);
  dst.byte[11] = (11 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[11]);
  dst.byte[12] = (12 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[12]);
  dst.byte[13] = (13 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[13]);
  dst.byte[14] = (14 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[14]);
  dst.byte[15] = (15 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[15]);
}
