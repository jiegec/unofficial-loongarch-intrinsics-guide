for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)b.half[i] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    u16 temp = (u16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  }
}
