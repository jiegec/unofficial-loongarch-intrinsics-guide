for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] % (u16)b.half[i]);
}
