dst.dword[0] = a.dword[(imm & 1) >> 0];
dst.dword[1] = a.dword[(imm & 2) >> 1];
dst.dword[2] = a.dword[2 + ((imm & 4) >> 2)];
dst.dword[3] = a.dword[2 + ((imm & 8) >> 3)];
