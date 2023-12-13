for (int i = 0; i < 32; i++) {
  dst.byte[i] = (u8)sadd((u8)a.byte[i], (u8)b.byte[i]);
}
