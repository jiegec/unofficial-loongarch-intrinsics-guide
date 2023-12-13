for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (u64)(u32)c.word[2 * i + 1] +
                 (u64)a.dword[i];
}
