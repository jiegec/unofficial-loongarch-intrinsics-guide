for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] % (s8)b.byte[i]);
}
