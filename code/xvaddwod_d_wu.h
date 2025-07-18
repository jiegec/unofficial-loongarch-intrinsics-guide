for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((u64)((u32)b.word[3]));
  dst.dword[2] = ((u64)((u32)a.word[5])) + ((u64)((u32)b.word[5]));
  dst.dword[3] = ((u64)((u32)a.word[7])) + ((u64)((u32)b.word[7]));
}
