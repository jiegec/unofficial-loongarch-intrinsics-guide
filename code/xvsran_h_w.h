for (int i = 0; i < 16; i++) {
  dst.half[i] = (i < 8) ? (s16)((s32)a.word[i] >> (b.word[i] & 31)) : 0;
}
