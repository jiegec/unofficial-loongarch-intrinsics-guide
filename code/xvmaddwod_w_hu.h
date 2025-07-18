for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)b.half[2 * i + 1] * (u32)(u16)c.half[2 * i + 1] +
                (u32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((u32)((u16)b.half[1])) * ((u32)((u16)c.half[1]))) + ((u32)a.word[0]);
  dst.word[1] =
      (((u32)((u16)b.half[3])) * ((u32)((u16)c.half[3]))) + ((u32)a.word[1]);
  dst.word[2] =
      (((u32)((u16)b.half[5])) * ((u32)((u16)c.half[5]))) + ((u32)a.word[2]);
  dst.word[3] =
      (((u32)((u16)b.half[7])) * ((u32)((u16)c.half[7]))) + ((u32)a.word[3]);
  dst.word[4] =
      (((u32)((u16)b.half[9])) * ((u32)((u16)c.half[9]))) + ((u32)a.word[4]);
  dst.word[5] =
      (((u32)((u16)b.half[11])) * ((u32)((u16)c.half[11]))) + ((u32)a.word[5]);
  dst.word[6] =
      (((u32)((u16)b.half[13])) * ((u32)((u16)c.half[13]))) + ((u32)a.word[6]);
  dst.word[7] =
      (((u32)((u16)b.half[15])) * ((u32)((u16)c.half[15]))) + ((u32)a.word[7]);
}
