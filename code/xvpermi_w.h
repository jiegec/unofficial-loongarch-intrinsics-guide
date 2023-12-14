dst.word[0] = b.word[imm & 0x3];
dst.word[1] = b.word[(imm >> 2) & 0x3];
dst.word[2] = a.word[(imm >> 4) & 0x3];
dst.word[3] = a.word[(imm >> 6) & 0x3];
dst.word[4] = b.word[4 + (imm & 0x3)];
dst.word[5] = b.word[4 + ((imm >> 2) & 0x3)];
dst.word[6] = a.word[4 + ((imm >> 4) & 0x3)];
dst.word[7] = a.word[4 + ((imm >> 6) & 0x3)];
