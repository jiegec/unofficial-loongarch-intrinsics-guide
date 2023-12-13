for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] <= imm) ? 0xFFFFFFFFFFFFFFFF : 0;
}
