for (int i = 0; i < 2; i++) {
  dst.dword[i] =
      (u64)(u32)b.word[2 * i] * (u64)(u32)c.word[2 * i] + (u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[0])) * ((u64)((u32)c.word[0]))) + ((u64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[2])) * ((u64)((u32)c.word[2]))) + ((u64)a.dword[1]);
}
