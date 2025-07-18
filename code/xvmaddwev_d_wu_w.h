for (int i = 0; i < 4; i++) {
  dst.dword[i] =
      (u64)(u32)b.word[2 * i] * (s64)(s32)c.word[2 * i] + (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[0])) * ((s64)((s32)c.word[0]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[2])) * ((s64)((s32)c.word[2]))) + ((s64)a.dword[1]);
  dst.dword[2] =
      (((u64)((u32)b.word[4])) * ((s64)((s32)c.word[4]))) + ((s64)a.dword[2]);
  dst.dword[3] =
      (((u64)((u32)b.word[6])) * ((s64)((s32)c.word[6]))) + ((s64)a.dword[3]);
}
