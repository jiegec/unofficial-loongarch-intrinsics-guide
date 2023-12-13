for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] < (u16)b.half[i]) ? 0xFFFF : 0;
}
