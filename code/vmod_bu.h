for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] % (u8)b.byte[i]);
}
