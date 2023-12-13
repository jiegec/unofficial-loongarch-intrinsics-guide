int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[1 + i];
}
for (; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 + i];
}
