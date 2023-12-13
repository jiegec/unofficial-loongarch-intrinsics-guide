for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (s16)a.half[i];
    } else {
      temp = ((s16)a.half[i] >> (b.half[i] & 15)) +
             (((s16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
