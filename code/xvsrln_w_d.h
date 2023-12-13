for (int i = 0; i < 8; i++) {
  dst.word[i] = (i < 4) ? (u32)((u64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
