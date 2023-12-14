for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[idx];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = a.word[idx + 4];
}
