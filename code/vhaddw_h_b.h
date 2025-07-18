for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] + (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) + ((s16)((s8)b.byte[0]));
  dst.half[1] = ((s16)((s8)a.byte[3])) + ((s16)((s8)b.byte[2]));
  dst.half[2] = ((s16)((s8)a.byte[5])) + ((s16)((s8)b.byte[4]));
  dst.half[3] = ((s16)((s8)a.byte[7])) + ((s16)((s8)b.byte[6]));
  dst.half[4] = ((s16)((s8)a.byte[9])) + ((s16)((s8)b.byte[8]));
  dst.half[5] = ((s16)((s8)a.byte[11])) + ((s16)((s8)b.byte[10]));
  dst.half[6] = ((s16)((s8)a.byte[13])) + ((s16)((s8)b.byte[12]));
  dst.half[7] = ((s16)((s8)a.byte[15])) + ((s16)((s8)b.byte[14]));
}
