for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] - (u32)(u16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) - ((u32)((u16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) - ((u32)((u16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) - ((u32)((u16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) - ((u32)((u16)b.half[7]));
}
