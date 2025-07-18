for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] - (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) - ((s128)((s64)b.dword[0]));
}
