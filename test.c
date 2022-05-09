#include <stdio.h>

#include "memblock.h"

int main() {
  char array[42];

  struct memblock int_memblock;
  init_mem(int_memblock, int, array, 10);

  int *p = use_mem(int, int_memblock);
  int *q = use_mem(int, int_memblock);
  int *r = use_mem(int, int_memblock);
  int *s = use_mem(int, int_memblock);
  int *t = use_mem(int, int_memblock);
  int *u = use_mem(int, int_memblock);
  int *v = use_mem(int, int_memblock);
  int *w = use_mem(int, int_memblock);
  int *x = use_mem(int, int_memblock);
  int *y = use_mem(int, int_memblock);

  *p = 100;
  *q = 101;
  *r = 102;
  *s = 103;
  *t = 104;
  *u = 105;
  *v = 106;
  *w = 107;
  *x = 108;
  *y = 109;

  printf("Data in memory locations:\n");
  printf("%d\n", ((int*)array)[0]);
  printf("%d\n", ((int*)array)[1]);
  printf("%d\n", ((int*)array)[2]);
  printf("%d\n", ((int*)array)[3]);
  printf("%d\n", ((int*)array)[4]);
  printf("%d\n", ((int*)array)[5]);
  printf("%d\n", ((int*)array)[6]);
  printf("%d\n", ((int*)array)[7]);
  printf("%d\n", ((int*)array)[8]);
  printf("%d\n", ((int*)array)[9]);

  int *z = (int*)use_mem(int, int_memblock);
  if (z == NULL) {
    printf("Memblock exhausted!\n");
  }

  free_mem(r, int_memblock);
  free_mem(u, int_memblock);
  free_mem(x, int_memblock);

  int *a = use_mem(int, int_memblock);
  *a = 1001;

  int *b = use_mem(int, int_memblock);
  *b = 9999;

  int *c = use_mem(int, int_memblock);
  *c = 2020;

  printf("Data after reusing memory locations:\n");
  printf("%d\n", ((int*)array)[8]);
  printf("%d\n", ((int*)array)[5]);
  printf("%d\n", ((int*)array)[2]);

  return 0;
}
