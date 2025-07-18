int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i];
}
for (; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[0]);
  dst.qword[1] = (u128)((u64)a.dword[2]);
}
