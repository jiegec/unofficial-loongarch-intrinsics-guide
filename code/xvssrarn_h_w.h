for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s32 temp;
    if ((b.word[i] & 31) == 0) {
      temp = (s32)a.word[i];
    } else {
      temp = ((s32)a.word[i] >> (b.word[i] & 31)) +
             (((s32)a.word[i] >> ((b.word[i] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    dst.half[i] = 0;
  }
}
