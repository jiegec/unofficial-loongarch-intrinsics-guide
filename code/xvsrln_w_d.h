for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (u32)((u64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] =
      (i < 6) ? (u32)((u64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) : 0;
}
