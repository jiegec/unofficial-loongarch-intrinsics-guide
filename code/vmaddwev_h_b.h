for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (s16)(s8)b.byte[2 * i] * (s16)(s8)c.byte[2 * i] + (s16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((s16)((s8)b.byte[0])) * ((s16)((s8)c.byte[0]))) + ((s16)a.half[0]);
  dst.half[1] =
      (((s16)((s8)b.byte[2])) * ((s16)((s8)c.byte[2]))) + ((s16)a.half[1]);
  dst.half[2] =
      (((s16)((s8)b.byte[4])) * ((s16)((s8)c.byte[4]))) + ((s16)a.half[2]);
  dst.half[3] =
      (((s16)((s8)b.byte[6])) * ((s16)((s8)c.byte[6]))) + ((s16)a.half[3]);
  dst.half[4] =
      (((s16)((s8)b.byte[8])) * ((s16)((s8)c.byte[8]))) + ((s16)a.half[4]);
  dst.half[5] =
      (((s16)((s8)b.byte[10])) * ((s16)((s8)c.byte[10]))) + ((s16)a.half[5]);
  dst.half[6] =
      (((s16)((s8)b.byte[12])) * ((s16)((s8)c.byte[12]))) + ((s16)a.half[6]);
  dst.half[7] =
      (((s16)((s8)b.byte[14])) * ((s16)((s8)c.byte[14]))) + ((s16)a.half[7]);
}
