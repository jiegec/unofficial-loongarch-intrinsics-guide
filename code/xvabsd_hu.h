for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] > (u16)b.half[i]) ? (a.half[i] - b.half[i])
                                                  : (b.half[i] - a.half[i]);
}
