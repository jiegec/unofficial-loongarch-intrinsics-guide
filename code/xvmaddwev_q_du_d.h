for (int i = 0; i < 2; i++) {
  dst.qword[i] =
      (u128)(u64)b.dword[2 * i] * (s128)(s64)c.dword[2 * i] + (s128)a.qword[i];
}
