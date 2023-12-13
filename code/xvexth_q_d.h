int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[1 + i];
}
for (; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 + i];
}
