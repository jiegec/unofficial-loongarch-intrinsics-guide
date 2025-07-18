int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[4 + i];
}
for (; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[8 + i];
}

// Expands to:

if (0) {
  dst.word[0] = (u32)((u16)a.half[4]);
  dst.word[1] = (u32)((u16)a.half[5]);
  dst.word[2] = (u32)((u16)a.half[6]);
  dst.word[3] = (u32)((u16)a.half[7]);
  dst.word[4] = (u32)((u16)a.half[12]);
  dst.word[5] = (u32)((u16)a.half[13]);
  dst.word[6] = (u32)((u16)a.half[14]);
  dst.word[7] = (u32)((u16)a.half[15]);
}
