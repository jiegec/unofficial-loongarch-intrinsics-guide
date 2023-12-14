for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] > (s32)b.word[i]) ? (a.word[i] - b.word[i])
                                                  : (b.word[i] - a.word[i]);
}
