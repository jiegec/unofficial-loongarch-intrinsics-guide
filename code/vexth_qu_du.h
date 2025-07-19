for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[1]);
}
