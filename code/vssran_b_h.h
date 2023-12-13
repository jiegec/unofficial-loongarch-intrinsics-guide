for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp = (s16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    dst.byte[i] = 0;
  }
}
