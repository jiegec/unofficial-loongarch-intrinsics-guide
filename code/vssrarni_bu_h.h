for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i];
    } else {
      temp = ((s16)b.half[i] >> imm) + (((s16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 8];
    } else {
      temp =
          ((s16)a.half[i - 8] >> imm) + (((s16)a.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  }
}
