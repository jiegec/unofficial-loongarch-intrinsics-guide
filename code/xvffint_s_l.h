for (int i = 0; i < 8; i++) {
  dst.fp32[i] =
      (i < 4) ? (f32)(s32)a.dword[i]
              : (f32)(s32)b.dword[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((s32)a.dword[0]);
  dst.fp32[1] = (f32)((s32)a.dword[1]);
  dst.fp32[2] = (f32)((s32)a.dword[2]);
  dst.fp32[3] = (f32)((s32)a.dword[3]);
  dst.fp32[4] = (f32)((s32)b.dword[4]);
  dst.fp32[5] = (f32)((s32)b.dword[5]);
  dst.fp32[6] = (f32)((s32)b.dword[6]);
  dst.fp32[7] = (f32)((s32)b.dword[7]);
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((s32)a.dword[0]);
  dst.fp32[1] = (f32)((s32)a.dword[1]);
  dst.fp32[2] = (f32)((s32)a.dword[2]);
  dst.fp32[3] = (f32)((s32)a.dword[3]);
  dst.fp32[4] = (f32)((s32)b.dword[4]);
  dst.fp32[5] = (f32)((s32)b.dword[5]);
  dst.fp32[6] = (f32)((s32)b.dword[6]);
  dst.fp32[7] = (f32)((s32)b.dword[7]);
}
