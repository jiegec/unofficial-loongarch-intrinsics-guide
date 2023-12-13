for (int i = 0; i < 32; i++) {
  if (i < 16) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i];
    } else {
      temp = ((s16)b.half[i] >> imm) + (((s16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 16];
    } else {
      temp = ((s16)a.half[i - 16] >> imm) +
             (((s16)a.half[i - 16] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  }
}
