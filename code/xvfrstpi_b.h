for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[imm % 16] = i;
for (i = 16; i < 32; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[(imm % 16) + 16] = i - 16;
