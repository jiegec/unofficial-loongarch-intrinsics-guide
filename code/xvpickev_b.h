for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? b.byte[i * 2] : a.byte[(i - 8) * 2];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? b.byte[(i - 8) * 2] : a.byte[(i - 16) * 2];
}
