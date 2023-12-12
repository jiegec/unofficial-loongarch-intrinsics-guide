for (int i = 0; i < 4; i++) {
  if ((b.word[i] & 0x1f) == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = ((s32)a.word[i] >> (b.word[i] & 0x1f)) +
                  (((s32)a.word[i] >> ((b.word[i] & 0x1f) - 1)) & 0x1);
  }
}
