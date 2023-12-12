for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] > (s8)b.byte[i]) ? (a.byte[i] - b.byte[i]) : (b.byte[i] - a.byte[i]);
}
