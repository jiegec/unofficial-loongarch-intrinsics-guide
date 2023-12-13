for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] >> 1) + ((u8)b.byte[i] >> 1) +
                ((a.byte[i] & b.byte[i]) & 1);
}
