for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    dst.half[i] = 0;
  }
}
