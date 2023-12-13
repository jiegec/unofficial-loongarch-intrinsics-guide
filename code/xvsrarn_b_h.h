for (int i = 0; i < 32; i++) {
  if (i < 16) {
    u8 shift = (b.half[i] & 15);
    if (shift == 0) {
      dst.byte[i] = (s8)(s16)a.half[i];
    } else {
      dst.byte[i] = (s8)(((s16)a.half[i] >> shift) +
                         (((s16)a.half[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
