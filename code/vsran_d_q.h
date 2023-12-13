for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (s64)((s128)a.qword[i] >> (b.qword[i] & 127)) : 0;
}
