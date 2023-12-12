for (int i = 0; i < 4; i++) {
  dst.word[i] = (((u64)(u32)a.word[i] * (u64)(u32)b.word[i])) >> 32;
}
