for (int i = 0; i < 8; i++) {
  if ((b.half[i] & 0xf) == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = (a.half[i] >> (b.half[i] & 0xf)) +
                  ((a.half[i] >> ((b.half[i] & 0xf) - 1)) & 0x1);
  }
}
