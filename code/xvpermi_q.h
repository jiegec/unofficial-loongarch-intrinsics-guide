if ((imm & 0x4) && (UARCH_LA264 || UARCH_LA464)) {
  // Caveat: observed in LA264 and LA464
  dst.qword[0] = 0;
} else {
  dst.qword[0] = (imm & 2) ? a.qword[imm & 0x1] : b.qword[imm & 0x1];
}
if ((imm & 0x80) && (UARCH_LA264 || UARCH_LA464)) {
  // Caveat: observed in LA264 and LA464
  dst.qword[1] = 0;
} else {
  dst.qword[1] =
      (imm & 0x20) ? a.qword[(imm >> 4) & 0x1] : b.qword[(imm >> 4) & 0x1];
}
