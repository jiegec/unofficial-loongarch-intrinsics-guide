for (int i = 0; i < 4; i++) {
  dst.word[i] =
      (a.word[i] == 0) ? 0 : ((s32)a.word[i] > 0 ? b.word[i] : -b.word[i]);
}
