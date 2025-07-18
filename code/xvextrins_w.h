int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i == ((imm >> 4) & 3)) ? b.word[imm & 3] : a.word[i];
}
for (; i < 8; i++) {
  dst.word[i] = (i - 4 == ((imm >> 4) & 3)) ? b.word[(imm & 3) + 4] : a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] = (0 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[0]);
  dst.word[1] = (1 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[1]);
  dst.word[2] = (2 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[2]);
  dst.word[3] = (3 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[3]);
  dst.word[4] = (0 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[4]);
  dst.word[5] = (1 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[5]);
  dst.word[6] = (2 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[6]);
  dst.word[7] = (3 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[7]);
}
