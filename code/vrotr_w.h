for (int i = 0; i < 4; i++) {
  dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) |
                (a.word[i] << (32 - (b.word[i] & 0x1f)));
}
