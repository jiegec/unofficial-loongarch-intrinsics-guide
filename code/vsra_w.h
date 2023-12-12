for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i]) >> (b.word[i] & 0x1f);
}
