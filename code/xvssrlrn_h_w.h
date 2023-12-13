for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u32 temp;
    if ((b.word[i] & 31) == 0) {
      temp = (u32)a.word[i];
    } else {
      temp = ((u32)a.word[i] >> (b.word[i] & 31)) +
             (((u32)a.word[i] >> ((b.word[i] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    dst.half[i] = 0;
  }
}
