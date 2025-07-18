for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] + (s16)(s8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) + ((s16)((s8)b.byte[1]));
  dst.half[1] = ((s16)((s8)a.byte[3])) + ((s16)((s8)b.byte[3]));
  dst.half[2] = ((s16)((s8)a.byte[5])) + ((s16)((s8)b.byte[5]));
  dst.half[3] = ((s16)((s8)a.byte[7])) + ((s16)((s8)b.byte[7]));
  dst.half[4] = ((s16)((s8)a.byte[9])) + ((s16)((s8)b.byte[9]));
  dst.half[5] = ((s16)((s8)a.byte[11])) + ((s16)((s8)b.byte[11]));
  dst.half[6] = ((s16)((s8)a.byte[13])) + ((s16)((s8)b.byte[13]));
  dst.half[7] = ((s16)((s8)a.byte[15])) + ((s16)((s8)b.byte[15]));
}
