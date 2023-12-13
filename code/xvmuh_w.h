for (int i = 0; i < 8; i++) {
  dst.word[i] = (((s64)(s32)a.word[i] * (s64)(s32)b.word[i])) >> 32;
}
