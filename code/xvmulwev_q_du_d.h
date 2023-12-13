for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] * (s128)(s64)b.dword[2 * i];
}
