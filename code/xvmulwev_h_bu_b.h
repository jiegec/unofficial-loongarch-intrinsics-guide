for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] * (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) * ((s16)((s8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) * ((s16)((s8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) * ((s16)((s8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) * ((s16)((s8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) * ((s16)((s8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) * ((s16)((s8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) * ((s16)((s8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) * ((s16)((s8)b.byte[14]));
  dst.half[8] = ((u16)((u8)a.byte[16])) * ((s16)((s8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[18])) * ((s16)((s8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[20])) * ((s16)((s8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[22])) * ((s16)((s8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[24])) * ((s16)((s8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[26])) * ((s16)((s8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[28])) * ((s16)((s8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[30])) * ((s16)((s8)b.byte[30]));
}
