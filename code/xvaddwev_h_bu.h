for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] + (u16)(u8)b.byte[2 * i];
}
