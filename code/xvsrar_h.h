for (int i = 0; i < 16; i++) {
  if ((b.half[i] & 0xf) == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = ((s16)a.half[i] >> (b.half[i] & 0xf)) +
                  (((s16)a.half[i] >> ((b.half[i] & 0xf) - 1)) & 0x1);
  }
}
