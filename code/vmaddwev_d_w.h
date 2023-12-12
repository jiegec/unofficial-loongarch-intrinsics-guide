for (int i = 0; i < 2; i++) {
  dst.dword[i] =
      (s64)(s32)b.word[2 * i] * (s64)(s32)c.word[2 * i] + (s64)a.dword[i];
}
