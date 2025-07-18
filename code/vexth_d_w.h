for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 + i];
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)((s32)a.word[2]);
  dst.dword[1] = (s64)((s32)a.word[3]);
}
