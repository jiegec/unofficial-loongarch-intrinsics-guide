for (int i = 0; i < 16; i++) {
  if (i < 8) {
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u8 shift = (b.half[i - 8] & 15);
    if (shift == 0) {
      dst.byte[i] = (s8)(s16)a.half[i - 8];
    } else {
      dst.byte[i] = (s8)(((s16)a.half[i - 8] >> shift) +
                         (((s16)a.half[i - 8] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
