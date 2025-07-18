for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] + (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) + ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[3])) + ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[5])) + ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[7])) + ((u32)((u16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[9])) + ((u32)((u16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[11])) + ((u32)((u16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[13])) + ((u32)((u16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[15])) + ((u32)((u16)b.half[14]));
}
