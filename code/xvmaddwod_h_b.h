for (int i = 0; i < 16; i++) {
  dst.half[i] =
      (s16)(s8)b.byte[2 * i + 1] * (s16)(s8)c.byte[2 * i + 1] + (s16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((s16)((s8)b.byte[1])) * ((s16)((s8)c.byte[1]))) + ((s16)a.half[0]);
  dst.half[1] =
      (((s16)((s8)b.byte[3])) * ((s16)((s8)c.byte[3]))) + ((s16)a.half[1]);
  dst.half[2] =
      (((s16)((s8)b.byte[5])) * ((s16)((s8)c.byte[5]))) + ((s16)a.half[2]);
  dst.half[3] =
      (((s16)((s8)b.byte[7])) * ((s16)((s8)c.byte[7]))) + ((s16)a.half[3]);
  dst.half[4] =
      (((s16)((s8)b.byte[9])) * ((s16)((s8)c.byte[9]))) + ((s16)a.half[4]);
  dst.half[5] =
      (((s16)((s8)b.byte[11])) * ((s16)((s8)c.byte[11]))) + ((s16)a.half[5]);
  dst.half[6] =
      (((s16)((s8)b.byte[13])) * ((s16)((s8)c.byte[13]))) + ((s16)a.half[6]);
  dst.half[7] =
      (((s16)((s8)b.byte[15])) * ((s16)((s8)c.byte[15]))) + ((s16)a.half[7]);
  dst.half[8] =
      (((s16)((s8)b.byte[17])) * ((s16)((s8)c.byte[17]))) + ((s16)a.half[8]);
  dst.half[9] =
      (((s16)((s8)b.byte[19])) * ((s16)((s8)c.byte[19]))) + ((s16)a.half[9]);
  dst.half[10] =
      (((s16)((s8)b.byte[21])) * ((s16)((s8)c.byte[21]))) + ((s16)a.half[10]);
  dst.half[11] =
      (((s16)((s8)b.byte[23])) * ((s16)((s8)c.byte[23]))) + ((s16)a.half[11]);
  dst.half[12] =
      (((s16)((s8)b.byte[25])) * ((s16)((s8)c.byte[25]))) + ((s16)a.half[12]);
  dst.half[13] =
      (((s16)((s8)b.byte[27])) * ((s16)((s8)c.byte[27]))) + ((s16)a.half[13]);
  dst.half[14] =
      (((s16)((s8)b.byte[29])) * ((s16)((s8)c.byte[29]))) + ((s16)a.half[14]);
  dst.half[15] =
      (((s16)((s8)b.byte[31])) * ((s16)((s8)c.byte[31]))) + ((s16)a.half[15]);
}
