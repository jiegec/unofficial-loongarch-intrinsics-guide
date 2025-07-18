for (int i = 0; i < 8; i++) {
  dst.half[i] = (i == ((imm >> 4) & 7)) ? b.half[imm & 7] : a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = (0 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[0]);
  dst.half[1] = (1 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[1]);
  dst.half[2] = (2 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[2]);
  dst.half[3] = (3 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[3]);
  dst.half[4] = (4 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[4]);
  dst.half[5] = (5 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[5]);
  dst.half[6] = (6 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[6]);
  dst.half[7] = (7 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[7]);
}
