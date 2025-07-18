for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] + (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) + ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) + ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) + ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) + ((u32)((u16)b.half[6]));
}
