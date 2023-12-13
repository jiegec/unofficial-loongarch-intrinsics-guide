for (int i = 0; i < 4; i++) {
  dst.dword[i] = (((u128)(u64)a.dword[i] * (u128)(u64)b.dword[i])) >> 64;
}
