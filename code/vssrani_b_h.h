for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp = (s16)b.half[i] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp = (s16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  }
}
