for (int i = 0; i < 8; i++) {
  if ((b.word[i] & 0x1f) == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) +
                  ((a.word[i] >> ((b.word[i] & 0x1f) - 1)) & 0x1);
  }
}
