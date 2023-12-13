for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (u16)a.half[i];
    } else {
      temp = ((u16)a.half[i] >> (b.half[i] & 15)) +
             (((u16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
