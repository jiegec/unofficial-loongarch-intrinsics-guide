for (int i = 0; i < 16; i++) {
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
  dst.half[8] =
      (((s16)((s8)b.byte[16])) * ((s16)((s8)c.byte[16]))) + ((s16)a.half[8]);
  dst.half[9] =
      (((s16)((s8)b.byte[18])) * ((s16)((s8)c.byte[18]))) + ((s16)a.half[9]);
  dst.half[10] =
      (((s16)((s8)b.byte[20])) * ((s16)((s8)c.byte[20]))) + ((s16)a.half[10]);
  dst.half[11] =
      (((s16)((s8)b.byte[22])) * ((s16)((s8)c.byte[22]))) + ((s16)a.half[11]);
  dst.half[12] =
      (((s16)((s8)b.byte[24])) * ((s16)((s8)c.byte[24]))) + ((s16)a.half[12]);
  dst.half[13] =
      (((s16)((s8)b.byte[26])) * ((s16)((s8)c.byte[26]))) + ((s16)a.half[13]);
  dst.half[14] =
      (((s16)((s8)b.byte[28])) * ((s16)((s8)c.byte[28]))) + ((s16)a.half[14]);
  dst.half[15] =
      (((s16)((s8)b.byte[30])) * ((s16)((s8)c.byte[30]))) + ((s16)a.half[15]);
}
