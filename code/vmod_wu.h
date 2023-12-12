for (int i = 0; i < 4; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((u32)a.word[i] % (u32)b.word[i]);
}
