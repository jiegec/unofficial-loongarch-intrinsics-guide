for (int i = 0; i < 32; i++) {
  dst.byte[i] = abs((s8)a.byte[i]) + abs((s8)b.byte[i]);
}
