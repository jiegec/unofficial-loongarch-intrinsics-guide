for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (u64)(u32)b.word[2 * i];
}
