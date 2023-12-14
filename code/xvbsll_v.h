int shift = (imm * 8) % 128;
dst.qword[0] = (u128)a.qword[0] << shift;
dst.qword[1] = (u128)a.qword[1] << shift;