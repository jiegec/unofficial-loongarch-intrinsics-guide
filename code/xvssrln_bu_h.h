for (int i = 0; i < 32; i++) {
  if (i < 16) {
    u16 temp = (u16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
