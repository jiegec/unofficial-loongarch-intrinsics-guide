for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] * (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) * ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) * ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) * ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) * ((u32)((u16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[8])) * ((u32)((u16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[10])) * ((u32)((u16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[12])) * ((u32)((u16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[14])) * ((u32)((u16)b.half[14]));
}
