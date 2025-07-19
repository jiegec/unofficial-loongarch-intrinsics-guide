for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8] << imm;
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[0])) << imm;
  dst.half[1] = ((s16)((s8)a.byte[1])) << imm;
  dst.half[2] = ((s16)((s8)a.byte[2])) << imm;
  dst.half[3] = ((s16)((s8)a.byte[3])) << imm;
  dst.half[4] = ((s16)((s8)a.byte[4])) << imm;
  dst.half[5] = ((s16)((s8)a.byte[5])) << imm;
  dst.half[6] = ((s16)((s8)a.byte[6])) << imm;
  dst.half[7] = ((s16)((s8)a.byte[7])) << imm;
  dst.half[8] = ((s16)((s8)a.byte[16])) << imm;
  dst.half[9] = ((s16)((s8)a.byte[17])) << imm;
  dst.half[10] = ((s16)((s8)a.byte[18])) << imm;
  dst.half[11] = ((s16)((s8)a.byte[19])) << imm;
  dst.half[12] = ((s16)((s8)a.byte[20])) << imm;
  dst.half[13] = ((s16)((s8)a.byte[21])) << imm;
  dst.half[14] = ((s16)((s8)a.byte[22])) << imm;
  dst.half[15] = ((s16)((s8)a.byte[23])) << imm;
}
