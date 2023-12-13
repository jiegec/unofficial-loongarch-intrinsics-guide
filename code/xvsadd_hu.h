for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)sadd((u16)a.half[i], (u16)b.half[i]);
}
