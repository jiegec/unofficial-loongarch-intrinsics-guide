for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[1])) * ((s64)((s32)c.word[1]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[3])) * ((s64)((s32)c.word[3]))) + ((s64)a.dword[1]);
}
