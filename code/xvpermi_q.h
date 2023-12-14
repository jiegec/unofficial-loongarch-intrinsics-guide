if ((imm & 0x4) && MACHINE_3C5000) {
  dst.qword[0] = 0;
} else {
  dst.qword[0] = (imm & 2) ? a.qword[imm & 0x1] : b.qword[imm & 0x1];
}
if ((imm & 0x80) && MACHINE_3C5000) {
  dst.qword[1] = 0;
} else {
  dst.qword[1] =
      (imm & 0x20) ? a.qword[(imm >> 4) & 0x1] : b.qword[(imm >> 4) & 0x1];
}
