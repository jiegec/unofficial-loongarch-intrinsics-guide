for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] >> 1) + ((u32)b.word[i] >> 1) +
                ((a.word[i] | b.word[i]) & 1);
}
