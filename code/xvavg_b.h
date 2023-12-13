for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] >> 1) + ((s8)b.byte[i] >> 1) +
                ((a.byte[i] & b.byte[i]) & 1);
}
