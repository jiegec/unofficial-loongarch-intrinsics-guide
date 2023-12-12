for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] > (u32)b.word[i]) ? (a.word[i] - b.word[i])
                                                  : (b.word[i] - a.word[i]);
}
