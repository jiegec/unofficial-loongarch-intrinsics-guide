for (int i = 0; i < 16; i++) {
  dst.byte[i] = abs((s8)a.byte[i]) + abs((s8)b.byte[i]);
}
