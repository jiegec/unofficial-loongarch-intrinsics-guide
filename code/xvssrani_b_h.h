for (int i = 0; i < 32; i++) {
  if (i < 16) {
    s16 temp = (s16)b.half[i] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp = (s16)a.half[i - 16] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  }
}
