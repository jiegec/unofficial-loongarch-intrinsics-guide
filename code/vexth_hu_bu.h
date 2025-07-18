for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[8 + i];
}

// Expands to:

if (0) {
  dst.half[0] = (u16)((u8)a.byte[8]);
  dst.half[1] = (u16)((u8)a.byte[9]);
  dst.half[2] = (u16)((u8)a.byte[10]);
  dst.half[3] = (u16)((u8)a.byte[11]);
  dst.half[4] = (u16)((u8)a.byte[12]);
  dst.half[5] = (u16)((u8)a.byte[13]);
  dst.half[6] = (u16)((u8)a.byte[14]);
  dst.half[7] = (u16)((u8)a.byte[15]);
}
