for (int i = 0; i < 1; i++) {
  dst.qword[i] =
      (u128)(u64)b.dword[2 * i] * (u128)(u64)c.dword[2 * i] + (u128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[0])) * ((u128)((u64)c.dword[0]))) +
                 ((u128)a.qword[0]);
}
