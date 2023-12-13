int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i];
}
for (; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i + 1];
}
