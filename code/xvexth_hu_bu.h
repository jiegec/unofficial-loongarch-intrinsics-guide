int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[8 + i];
}
for (; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[16 + i];
}
