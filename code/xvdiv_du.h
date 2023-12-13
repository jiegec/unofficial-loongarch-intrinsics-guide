for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((u64)a.dword[i] / (u64)b.dword[i]);
}
