for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp;
    if (imm == 0) {
      temp = (u16)b.half[i];
    } else {
      temp = ((u16)b.half[i] >> imm) + (((u16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    u16 temp;
    if (imm == 0) {
      temp = (u16)a.half[i - 8];
    } else {
      temp =
          ((u16)a.half[i - 8] >> imm) + (((u16)a.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  }
}
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp;
    if (imm == 0) {
      temp = (u16)b.half[i - 8];
    } else {
      temp =
          ((u16)b.half[i - 8] >> imm) + (((u16)b.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    u16 temp;
    if (imm == 0) {
      temp = (u16)a.half[i - 16];
    } else {
      temp = ((u16)a.half[i - 16] >> imm) +
             (((u16)a.half[i - 16] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  }
}
