for (int i = 0; i < 4; i++) {
  if ((b.dword[i] & 0x3f) == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] = (a.dword[i] >> (b.dword[i] & 0x3f)) +
                   ((a.dword[i] >> ((b.dword[i] & 0x3f) - 1)) & 0x1);
  }
}
