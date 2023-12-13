for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)b.dword[2 * i + 1] * (u128)(u64)c.dword[2 * i + 1] +
                 (u128)a.qword[i];
}
