for (int i = 0; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] - (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) - ((s16)((s8)b.byte[0]));
  dst.half[1] = ((s16)((s8)a.byte[3])) - ((s16)((s8)b.byte[2]));
  dst.half[2] = ((s16)((s8)a.byte[5])) - ((s16)((s8)b.byte[4]));
  dst.half[3] = ((s16)((s8)a.byte[7])) - ((s16)((s8)b.byte[6]));
  dst.half[4] = ((s16)((s8)a.byte[9])) - ((s16)((s8)b.byte[8]));
  dst.half[5] = ((s16)((s8)a.byte[11])) - ((s16)((s8)b.byte[10]));
  dst.half[6] = ((s16)((s8)a.byte[13])) - ((s16)((s8)b.byte[12]));
  dst.half[7] = ((s16)((s8)a.byte[15])) - ((s16)((s8)b.byte[14]));
  dst.half[8] = ((s16)((s8)a.byte[17])) - ((s16)((s8)b.byte[16]));
  dst.half[9] = ((s16)((s8)a.byte[19])) - ((s16)((s8)b.byte[18]));
  dst.half[10] = ((s16)((s8)a.byte[21])) - ((s16)((s8)b.byte[20]));
  dst.half[11] = ((s16)((s8)a.byte[23])) - ((s16)((s8)b.byte[22]));
  dst.half[12] = ((s16)((s8)a.byte[25])) - ((s16)((s8)b.byte[24]));
  dst.half[13] = ((s16)((s8)a.byte[27])) - ((s16)((s8)b.byte[26]));
  dst.half[14] = ((s16)((s8)a.byte[29])) - ((s16)((s8)b.byte[28]));
  dst.half[15] = ((s16)((s8)a.byte[31])) - ((s16)((s8)b.byte[30]));
}
