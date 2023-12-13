for (int i = 0; i < 32; i++) {
  if (i < 16) {
    u16 temp = (u16)b.half[i] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    u16 temp = (u16)a.half[i - 16] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  }
}
