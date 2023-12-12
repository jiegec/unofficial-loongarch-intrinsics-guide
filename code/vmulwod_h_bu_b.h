for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] * (s16)(s8)b.byte[2 * i + 1];
}
