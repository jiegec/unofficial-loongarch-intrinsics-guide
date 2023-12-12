for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] < (u16)b.half[i]) ? 0xFFFF : 0;
}
