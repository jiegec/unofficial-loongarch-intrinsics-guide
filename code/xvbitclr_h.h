for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] & (~((u16)1 << (b.half[i] % 16)));
}
