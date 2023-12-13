for (int i = 0; i < 32; i++) {
  dst.byte[i] = b.byte[i] * c.byte[i] + a.byte[i];
}
