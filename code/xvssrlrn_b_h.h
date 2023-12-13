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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp;
    if ((b.half[i - 8] & 15) == 0) {
      temp = (u16)a.half[i - 8];
    } else {
      temp = ((u16)a.half[i - 8] >> (b.half[i - 8] & 15)) +
             (((u16)a.half[i - 8] >> ((b.half[i - 8] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
