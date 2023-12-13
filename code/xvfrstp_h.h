for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i];
}
int i;
for (i = 0; i < 8; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[c.half[0] % 8] = i;
for (i = 8; i < 16; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[(c.half[8] % 8) + 8] = i - 8;
