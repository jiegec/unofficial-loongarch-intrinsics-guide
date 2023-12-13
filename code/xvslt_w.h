for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] < (s32)b.word[i]) ? 0xFFFFFFFF : 0;
}
