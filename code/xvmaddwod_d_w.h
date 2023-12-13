for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}
