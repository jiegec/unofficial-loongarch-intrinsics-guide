for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (a.byte[i] == 0) ? 0 : ((s8)a.byte[i] > 0 ? b.byte[i] : -b.byte[i]);
}
