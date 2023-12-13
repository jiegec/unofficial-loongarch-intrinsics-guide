for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u8 shift = (b.word[i] & 31);
    if (shift == 0) {
      dst.half[i] = (u16)(u32)a.word[i];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i] >> shift) +
                          (((u32)a.word[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
  }
}
