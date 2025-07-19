for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(u32)a.word[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((u32)a.word[0]);
  dst.fp32[1] = (f32)((u32)a.word[1]);
  dst.fp32[2] = (f32)((u32)a.word[2]);
  dst.fp32[3] = (f32)((u32)a.word[3]);
  dst.fp32[4] = (f32)((u32)a.word[4]);
  dst.fp32[5] = (f32)((u32)a.word[5]);
  dst.fp32[6] = (f32)((u32)a.word[6]);
  dst.fp32[7] = (f32)((u32)a.word[7]);
}
