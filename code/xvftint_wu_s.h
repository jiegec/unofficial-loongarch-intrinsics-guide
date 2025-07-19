for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (u32)a.fp32[0];
  dst.word[1] = (u32)a.fp32[1];
  dst.word[2] = (u32)a.fp32[2];
  dst.word[3] = (u32)a.fp32[3];
  dst.word[4] = (u32)a.fp32[4];
  dst.word[5] = (u32)a.fp32[5];
  dst.word[6] = (u32)a.fp32[6];
  dst.word[7] = (u32)a.fp32[7];
}
