for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] & (~((u16)1 << (b.half[i] % 16)));
}
