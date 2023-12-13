for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i];
}
int i;
for (i = 0; i < 8; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[c.half[0] % 8] = i;
