for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 4];
}

// Expands to:

if (0) {
  dst.word[0] = (u32)((u16)a.half[4]);
  dst.word[1] = (u32)((u16)a.half[5]);
  dst.word[2] = (u32)((u16)a.half[6]);
  dst.word[3] = (u32)((u16)a.half[7]);
}
