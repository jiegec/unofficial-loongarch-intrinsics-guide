for (int i = 0; i < 2; i++) {
  dst.qword[i] =
      (s128)(s64)b.dword[2 * i] * (s128)(s64)c.dword[2 * i] + (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((s128)((s64)b.dword[0])) * ((s128)((s64)c.dword[0]))) +
                 ((s128)a.qword[0]);
  dst.qword[1] = (((s128)((s64)b.dword[2])) * ((s128)((s64)c.dword[2]))) +
                 ((s128)a.qword[1]);
}
