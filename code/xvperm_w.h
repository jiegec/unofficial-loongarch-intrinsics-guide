for (int i = 0;i < 8;i++) {
  dst.word[i] = a.word[b.word[i] & 0x8];
}