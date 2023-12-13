for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)b.dword[2 * i + 1] * (s128)(s64)c.dword[2 * i + 1] +
                 (s128)a.qword[i];
}
