for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[i] << imm;
}
