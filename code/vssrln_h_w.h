for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    dst.half[i] = 0;
  }
}
