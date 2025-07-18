for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (s128)((s64)a.dword[0]);
}
