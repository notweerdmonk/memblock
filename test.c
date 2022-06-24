#include <stdio.h>
#include <stdlib.h>

#include <memblock.h>

void test1() {
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

  printf("test 1\n");

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
    printf("Memblock exhausted\n");
  }

  free_mem(r, int_memblock);
  free_mem(s, int_memblock);
  free_mem(x, int_memblock);

  int *a = use_mem(int, int_memblock);
  *a = 1001;

  int *b = use_mem(int, int_memblock);
  *b = 9999;

  int *c = use_mem(int, int_memblock);
  *c = 2020;

  printf("Data after reusing memory locations:\n");
  printf("%d\n", ((int*)array)[8]);
  printf("%d\n", ((int*)array)[3]);
  printf("%d\n", ((int*)array)[2]);
}

/* struct of size 16 bytes with padding */
struct foo {
  int n;
  double f;
};

void test2() {
  int *array = (int*)malloc(64 * sizeof(int));

  struct memblock foo_memblock;
  init_mem(foo_memblock, struct foo, array, 4);

  struct foo *p = use_mem(struct foo, foo_memblock);
  struct foo *q = use_mem(struct foo, foo_memblock);
  struct foo *r = use_mem(struct foo, foo_memblock);
  struct foo *s = use_mem(struct foo, foo_memblock);

  p->n = 101;
  p->f = 100.1;
  q->n = 102;
  q->f = 100.2;
  r->n = 103;
  r->f = 100.3;
  s->n = 104;
  s->f = 100.4;

  printf("test 2\n");

  printf("Data in memory locations:\n");
  printf("%d %f\n", ((struct foo*)array)[0].n, ((struct foo*)array)[0].f);
  printf("%d %f\n", ((struct foo*)array)[1].n, ((struct foo*)array)[1].f);
  printf("%d %f\n", ((struct foo*)array)[2].n, ((struct foo*)array)[2].f);
  printf("%d %f\n", ((struct foo*)array)[3].n, ((struct foo*)array)[3].f);
  printf("%d %f\n", ((struct foo*)array)[4].n, ((struct foo*)array)[4].f);

  struct foo *t = use_mem(struct foo, foo_memblock);
  if (t == NULL) {
    printf("Memblock exhausted\n");
  }

  free_mem(p, foo_memblock);
  free_mem(r, foo_memblock);

  struct foo *a = use_mem(struct foo, foo_memblock);
  a->n = 1001;
  a->f = 1000.1;

  struct foo *b = use_mem(struct foo, foo_memblock);
  b->n = 9999;
  b->f = 9999.9;

  printf("Data after reusing memory locations:\n");
  printf("%d %f\n", ((struct foo*)array)[2].n, ((struct foo*)array)[2].f);
  printf("%d %f\n", ((struct foo*)array)[0].n, ((struct foo*)array)[0].f);

  free(array);
}

int main() {
  test1();
  test2();

  return 0;
}
