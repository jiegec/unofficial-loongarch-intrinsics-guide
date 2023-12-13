for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
