for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 8] << imm;
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) << imm;
  dst.half[1] = ((u16)((u8)a.byte[1])) << imm;
  dst.half[2] = ((u16)((u8)a.byte[2])) << imm;
  dst.half[3] = ((u16)((u8)a.byte[3])) << imm;
  dst.half[4] = ((u16)((u8)a.byte[4])) << imm;
  dst.half[5] = ((u16)((u8)a.byte[5])) << imm;
  dst.half[6] = ((u16)((u8)a.byte[6])) << imm;
  dst.half[7] = ((u16)((u8)a.byte[7])) << imm;
  dst.half[8] = ((u16)((u8)a.byte[16])) << imm;
  dst.half[9] = ((u16)((u8)a.byte[17])) << imm;
  dst.half[10] = ((u16)((u8)a.byte[18])) << imm;
  dst.half[11] = ((u16)((u8)a.byte[19])) << imm;
  dst.half[12] = ((u16)((u8)a.byte[20])) << imm;
  dst.half[13] = ((u16)((u8)a.byte[21])) << imm;
  dst.half[14] = ((u16)((u8)a.byte[22])) << imm;
  dst.half[15] = ((u16)((u8)a.byte[23])) << imm;
}
