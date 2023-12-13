for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] * (u32)(u16)b.half[2 * i + 1];
}
