for (int i = 0; i < 4; i++) {
  dst.fp32[i] = __builtin_scalbn(a.fp32[i], b.word[i]);
}
