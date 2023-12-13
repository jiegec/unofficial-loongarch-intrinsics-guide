for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] << (b.byte[i] & 0x7);
}
