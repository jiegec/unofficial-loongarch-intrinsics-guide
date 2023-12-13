for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[idx % 16];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = a.byte[(idx % 16) + 16];
}
