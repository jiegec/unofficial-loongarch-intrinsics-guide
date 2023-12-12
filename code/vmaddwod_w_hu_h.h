for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)b.half[2 * i + 1] * (s32)(s16)c.half[2 * i + 1] +
                (s32)a.word[i];
}
