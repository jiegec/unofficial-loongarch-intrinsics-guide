for (int i = 0; i < 2; i++) {
  if (abs(a.fp64[i]) > abs(b.fp64[i])) {
    dst.fp64[i] = a.fp64[i];
  } else if (abs(b.fp64[i]) > abs(a.fp64[i])) {
    dst.fp64[i] = b.fp64[i];
  } else {
    dst.fp64[i] = fmax(a.fp64[i], b.fp64[i]);
  }
}
