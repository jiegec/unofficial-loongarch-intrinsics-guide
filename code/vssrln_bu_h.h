for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
