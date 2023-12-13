for (int i = 0; i < 16; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i] * (s16)(s8)c.byte[2 * i] + (s16)a.half[i];
}
