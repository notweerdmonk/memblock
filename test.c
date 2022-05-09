#include <stdio.h>

#include "memblock.h"

int main() {
  int array[10];

  struct memblock int_memblock;
  init_mem(int_memblock, array, 10);

  int *p = (int*)use_mem(int, int_memblock);
  int *q = (int*)use_mem(int, int_memblock);
  int *r = (int*)use_mem(int, int_memblock);
  int *s = (int*)use_mem(int, int_memblock);
  int *t = (int*)use_mem(int, int_memblock);
  int *u = (int*)use_mem(int, int_memblock);
  int *v = (int*)use_mem(int, int_memblock);
  int *w = (int*)use_mem(int, int_memblock);
  int *x = (int*)use_mem(int, int_memblock);
  int *y = (int*)use_mem(int, int_memblock);

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
  printf("%d\n", array[0]);
  printf("%d\n", array[1]);
  printf("%d\n", array[2]);
  printf("%d\n", array[3]);
  printf("%d\n", array[4]);
  printf("%d\n", array[5]);
  printf("%d\n", array[6]);
  printf("%d\n", array[7]);
  printf("%d\n", array[8]);
  printf("%d\n", array[9]);

  int *z = (int*)use_mem(int, int_memblock);
  if (z == NULL) {
    printf("Memblock exhausted!\n");
  }

  free_mem(r, int_memblock);
  free_mem(u, int_memblock);
  free_mem(x, int_memblock);

  int *a = (int*)use_mem(int, int_memblock);
  *a = 1001;

  int *b = (int*)use_mem(int, int_memblock);
  *b = 9999;

  int *c = (int*)use_mem(int, int_memblock);
  *c = 2020;

  printf("Data after reusing memory locations:\n");
  printf("%d\n", array[8]);
  printf("%d\n", array[5]);
  printf("%d\n", array[2]);

  return 0;
}
