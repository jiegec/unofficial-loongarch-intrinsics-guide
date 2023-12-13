for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[c.byte[0] % 16] = i;
