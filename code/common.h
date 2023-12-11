#include <lsxintrin.h>
#include <stdio.h>

void test();

void print(const char *s, __m128i num) {
  printf("__m128i %s: %016llx %016llx\n", s, num[0], num[1]);
}

void print(const char *s, __m128 num) {
  printf("__m128 %s: %f %f %f %f\n", s, num[0], num[1], num[2], num[3]);
}

void print(const char *s, __m128d num) {
  printf("__m128d %s: %lf %lf\n", s, num[0], num[1]);
}

#define PRINT(x) print(#x, x)

int main(int argc, char *argv[]) {
  printf("Testing %s\n", argv[0]);
  test();
  return 0;
}
