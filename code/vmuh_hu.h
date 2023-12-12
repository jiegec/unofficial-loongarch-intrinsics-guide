for (int i = 0; i < 8; i++) {
  dst.half[i] = (((u32)(u16)a.half[i] * (u32)(u16)b.half[i])) >> 16;
}
