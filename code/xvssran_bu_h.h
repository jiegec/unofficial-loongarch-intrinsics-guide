for (int i = 0; i < 32; i++) {
  if (i < 16) {
    s16 temp = (s16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
