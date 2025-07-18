for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] + (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) + ((u128)((u64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[2])) + ((u128)((u64)b.dword[2]));
}
