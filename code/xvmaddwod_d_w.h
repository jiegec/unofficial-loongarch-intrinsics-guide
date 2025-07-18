for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((s64)((s32)b.word[1])) * ((s64)((s32)c.word[1]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((s64)((s32)b.word[3])) * ((s64)((s32)c.word[3]))) + ((s64)a.dword[1]);
  dst.dword[2] =
      (((s64)((s32)b.word[5])) * ((s64)((s32)c.word[5]))) + ((s64)a.dword[2]);
  dst.dword[3] =
      (((s64)((s32)b.word[7])) * ((s64)((s32)c.word[7]))) + ((s64)a.dword[3]);
}
