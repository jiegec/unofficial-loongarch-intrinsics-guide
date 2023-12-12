for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] | ((u8)1 << (b.byte[i] % 8));
}
