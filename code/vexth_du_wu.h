for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[i + 2];
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)((u32)a.word[2]);
  dst.dword[1] = (u64)((u32)a.word[3]);
}
