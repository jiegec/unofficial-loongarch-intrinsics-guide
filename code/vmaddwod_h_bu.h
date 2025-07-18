for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i + 1] * (u16)(u8)c.byte[2 * i + 1] + (u16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[1])) * ((u16)((u8)c.byte[1]))) + ((u16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[3])) * ((u16)((u8)c.byte[3]))) + ((u16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[5])) * ((u16)((u8)c.byte[5]))) + ((u16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[7])) * ((u16)((u8)c.byte[7]))) + ((u16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[9])) * ((u16)((u8)c.byte[9]))) + ((u16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[11])) * ((u16)((u8)c.byte[11]))) + ((u16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[13])) * ((u16)((u8)c.byte[13]))) + ((u16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[15])) * ((u16)((u8)c.byte[15]))) + ((u16)a.half[7]);
}
