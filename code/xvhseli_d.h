dst.dword[0] = (imm & 1) ? a.dword[1] : a.dword[0];
dst.dword[1] = (imm & 2) ? a.dword[1] : a.dword[0];
dst.dword[2] = (imm & 4) ? a.dword[3] : a.dword[2];
dst.dword[3] = (imm & 8) ? a.dword[3] : a.dword[2];
