for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] & (~((u32)1 << (b.word[i] % 32)));
}
