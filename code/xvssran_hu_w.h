for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s32 temp = (s32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
