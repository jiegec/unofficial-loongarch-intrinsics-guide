for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u8 shift = (b.half[i] & 15);
    if (shift == 0) {
      dst.byte[i] = (u8)(u16)a.half[i];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i] >> shift) +
                         (((u16)a.half[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
