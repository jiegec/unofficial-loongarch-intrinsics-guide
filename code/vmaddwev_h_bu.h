for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i] * (u16)(u8)c.byte[2 * i] + (u16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[0])) * ((u16)((u8)c.byte[0]))) + ((u16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[2])) * ((u16)((u8)c.byte[2]))) + ((u16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[4])) * ((u16)((u8)c.byte[4]))) + ((u16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[6])) * ((u16)((u8)c.byte[6]))) + ((u16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[8])) * ((u16)((u8)c.byte[8]))) + ((u16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[10])) * ((u16)((u8)c.byte[10]))) + ((u16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[12])) * ((u16)((u8)c.byte[12]))) + ((u16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[14])) * ((u16)((u8)c.byte[14]))) + ((u16)a.half[7]);
}
