for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (a.byte[i] >> (b.byte[i] & 0x7)) | (a.byte[i] << (8 - (b.byte[i] & 0x7)));
}
