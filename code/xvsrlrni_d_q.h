for (int i = 0; i < 2; i++) {
  if (i < 1) {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)b.qword[i];
    } else {
      dst.dword[i] = (u64)(((u128)b.qword[i] >> imm) +
                           (((u128)b.qword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)a.qword[i - 1];
    } else {
      dst.dword[i] = (u64)(((u128)a.qword[i - 1] >> imm) +
                           (((u128)a.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  }
}
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)b.qword[i - 1];
    } else {
      dst.dword[i] = (u64)(((u128)b.qword[i - 1] >> imm) +
                           (((u128)b.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)a.qword[i - 2];
    } else {
      dst.dword[i] = (u64)(((u128)a.qword[i - 2] >> imm) +
                           (((u128)a.qword[i - 2] >> (imm - 1)) & 0x1));
    }
  }
}
