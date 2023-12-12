for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : (s8)a.byte[i] / ((s8)b.byte[i]);
}
