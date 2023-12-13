for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] % (u16)b.half[i]);
}
