dst.dword[0] = (imm & 2) ? b.dword[(imm & 1)] : a.dword[(imm & 1)];
dst.dword[1] =
    (imm & 8) ? b.dword[((imm >> 2) & 1)] : a.dword[((imm >> 2) & 1)];
dst.dword[2] = (imm & 2) ? b.dword[(imm & 1) + 2] : a.dword[(imm & 1) + 2];
dst.dword[3] =
    (imm & 8) ? b.dword[((imm >> 2) & 1) + 2] : a.dword[((imm >> 2) & 1) + 2];