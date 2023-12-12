for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] > (u8)b.byte[i]) ? (a.byte[i] - b.byte[i]) : (b.byte[i] - a.byte[i]);
}
