for (int i = 0; i < 4; i++) {
  if (fp_compare_cule(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
