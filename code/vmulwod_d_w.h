for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] * (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) * ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) * ((s64)((s32)b.word[3]));
}
