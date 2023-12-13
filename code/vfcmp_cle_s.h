for (int i = 0; i < 4; i++) {
  if (fp_compare_cle(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
