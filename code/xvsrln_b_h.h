for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? (u8)((u16)a.half[i] >> (b.half[i] & 15)) : 0;
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (u8)((u16)a.half[i - 8] >> (b.half[i - 8] & 15)) : 0;
}
