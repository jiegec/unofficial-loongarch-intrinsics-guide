for (int i = 0; i < 4; i++) {
  dst.dword[i] = (a.dword[i] >> (b.dword[i] & 0x3f)) |
                 (a.dword[i] << (64 - (b.dword[i] & 0x3f)));
}
