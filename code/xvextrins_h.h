int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (i == ((imm >> 4) & 7)) ? b.half[imm & 7] : a.half[i];
}
for (; i < 16; i++) {
  dst.half[i] = (i - 8 == ((imm >> 4) & 7)) ? b.half[(imm & 7) + 8] : a.half[i];
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
  dst.half[8] = (0 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[8]);
  dst.half[9] = (1 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[9]);
  dst.half[10] =
      (2 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[10]);
  dst.half[11] =
      (3 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[11]);
  dst.half[12] =
      (4 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[12]);
  dst.half[13] =
      (5 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[13]);
  dst.half[14] =
      (6 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[14]);
  dst.half[15] =
      (7 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[15]);
}
