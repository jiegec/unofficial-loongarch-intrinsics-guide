for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] * (s64)(s32)b.word[2 * i + 1];
}
