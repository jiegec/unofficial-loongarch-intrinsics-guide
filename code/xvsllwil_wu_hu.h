for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i] << imm;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 4] << imm;
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) << imm;
  dst.word[1] = ((u32)((u16)a.half[1])) << imm;
  dst.word[2] = ((u32)((u16)a.half[2])) << imm;
  dst.word[3] = ((u32)((u16)a.half[3])) << imm;
  dst.word[4] = ((u32)((u16)a.half[8])) << imm;
  dst.word[5] = ((u32)((u16)a.half[9])) << imm;
  dst.word[6] = ((u32)((u16)a.half[10])) << imm;
  dst.word[7] = ((u32)((u16)a.half[11])) << imm;
}
