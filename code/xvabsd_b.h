for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] > (s8)b.byte[i]) ? (a.byte[i] - b.byte[i])
                                                : (b.byte[i] - a.byte[i]);
}
