for (int i = 0; i < 16; i++) {
  dst.byte[i] = b.byte[i] * c.byte[i] + a.byte[i];
}
