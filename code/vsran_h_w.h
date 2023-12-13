for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? (s16)((s32)a.word[i] >> (b.word[i] & 31)) : 0;
}
