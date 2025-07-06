for (int i = 0; i < 8; i++) {
  if (i < 2) {
    dst.fp32[i] = b.fp64[i];
  } else if(i < 4){
    dst.fp32[i] = a.fp64[i - 2];
  } else if(i < 6){
    dst.fp32[i] = b.fp64[i - 2];
  } else {
    dst.fp32[i] = a.fp64[i - 4];
  }
}
