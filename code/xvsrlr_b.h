for (int i = 0; i < 32; i++) {
  if ((b.byte[i] & 0x7) == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = (a.byte[i] >> (b.byte[i] & 0x7)) +
                  ((a.byte[i] >> ((b.byte[i] & 0x7) - 1)) & 0x1);
  }
}
