#include <lsxintrin.h>
#include <stdio.h>

void test();

void print(const char *s, __m128 num) {
  printf("%s: %f %f %f %f\n", s, num[0], num[1], num[2], num[3]);
}

void print(const char *s, __m128d num) {
  printf("%s: %lf %lf\n", s, num[0], num[1]);
}

#define PRINT(x) print(#x, x)

int main(int argc, char *argv[]) {
  printf("Testing %s\n", argv[0]);
  test();
  return 0;
}
