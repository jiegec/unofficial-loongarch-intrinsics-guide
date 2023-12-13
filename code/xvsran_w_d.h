for (int i = 0; i < 8; i++) {
  dst.word[i] = (i < 4) ? (s32)((s64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
