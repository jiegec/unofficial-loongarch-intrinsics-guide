for (int i = 0; i < 16; i++) {
  if (i < 8) {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)b.half[i];
    } else {
      dst.byte[i] =
          (u8)(((u16)b.half[i] >> imm) + (((u16)b.half[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)a.half[i - 8];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i - 8] >> imm) +
                         (((u16)a.half[i - 8] >> (imm - 1)) & 0x1));
    }
  }
}
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)b.half[i - 8];
    } else {
      dst.byte[i] = (u8)(((u16)b.half[i - 8] >> imm) +
                         (((u16)b.half[i - 8] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)a.half[i - 16];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i - 16] >> imm) +
                         (((u16)a.half[i - 16] >> (imm - 1)) & 0x1));
    }
  }
}
