dst.dword[0] = a.dword[imm & 0x3];
dst.dword[1] = a.dword[(imm >> 2) & 0x3];
dst.dword[2] = a.dword[(imm >> 4) & 0x3];
dst.dword[3] = a.dword[(imm >> 6) & 0x3];
