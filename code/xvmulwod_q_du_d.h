for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] * (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) * ((s128)((s64)b.dword[1]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) * ((s128)((s64)b.dword[3]));
}
