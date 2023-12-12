for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] - (u128)(u64)b.dword[2 * i + 1];
}
