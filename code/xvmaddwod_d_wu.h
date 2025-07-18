for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (u64)(u32)c.word[2 * i + 1] +
                 (u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[1])) * ((u64)((u32)c.word[1]))) + ((u64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[3])) * ((u64)((u32)c.word[3]))) + ((u64)a.dword[1]);
  dst.dword[2] =
      (((u64)((u32)b.word[5])) * ((u64)((u32)c.word[5]))) + ((u64)a.dword[2]);
  dst.dword[3] =
      (((u64)((u32)b.word[7])) * ((u64)((u32)c.word[7]))) + ((u64)a.dword[3]);
}
