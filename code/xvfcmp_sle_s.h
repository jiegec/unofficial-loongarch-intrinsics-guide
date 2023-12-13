for (int i = 0; i < 8; i++) {
  if (fp_compare_sle(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
