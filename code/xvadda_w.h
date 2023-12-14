for (int i = 0; i < 8; i++) {
  dst.word[i] = abs((s32)a.word[i]) + abs((s32)b.word[i]);
}
