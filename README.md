memblock
========

A memory pool manager.

Let us consider a contiguous block of memory which is either statically allocated on the stack of dynamically allocated on the heap. You need to use equal sized chunks from this memory block and memory locations can be reused.

```c
struct foo {
  int n;
  double f;
};

int array[4 * sizeof(struct foo)];
```

Initialize the memory pool:

```c
struct memblock mempool;
init_mem(mempool, struct foo, array, 4);
```

Now you can use chunks of memory:

```c
struct foo *p = use_mem(struct foo, mempool);
struct foo *q = use_mem(struct foo, mempool);
struct foo *r = use_mem(struct foo, mempool);
struct foo *s = use_mem(struct foo, mempool);
struct foo *t = use_mem(struct foo, mempool);
if (t != NULL) {
  // never reached as t will be NULL because the memory pool is exhausted
}
```

And free any chunk so that it can be reused without bookkeeping:

```c
free(p, mempool);
free(q, mempool);
free(s, mempool);
```

Consider checking `memblock.h` for documentation on the arguments and `test.c` for examples of usage.

Here is an insight of how memblock works internally:

```c
 On initialization of memblock start and avail point to the first memory element
  and end points the last element. Every element contains the offset to next
  element which is the size of one element.

       start                                        end
       avail                                         |
         |                                           |
        \|/                                         \|/
  ___________________________________________________________
 |              |              |              |              |
 | sizeof(type) | sizeof(type) | sizeof(type) | guard value  |
 |______________|______________|______________|______________|

 After using first memory element avail points to the next element.
 avail = avail + *avail

       start                                        end
         |            avail                          |
         |              |                            |
        \|/            \|/                          \|/
  ___________________________________________________________
 |              |              |              |              |
 |     data     | sizeof(type) | sizeof(type) | guard value  |
 |______________|______________|______________|______________|

 When avail is pointing the the last element it contains a guard value.
 
       start                                        end
         |                                         avail
         |                                           |
        \|/                                         \|/
  ___________________________________________________________
 |              |              |              |              |
 |     data     |     data     |     data     | guard value  |
 |______________|______________|______________|______________|

 After last element has been used avail will point to 0.

       start                                        end
         |                                           |             avail
         |                                           |               |
        \|/                                         \|/             \|/
  ___________________________________________________________
 |              |              |              |              |
 |     data     |     data     |     data     |     data     |       0
 |______________|______________|______________|______________|

 When the first location is freed its contents are set to a guard value.

       start                                        end
         |                           avail           |
         |                             |             |
        \|/                           \|/           \|/
  ___________________________________________________________
 |              |              |              |              |
 |     data     |     data     | guard value  |     data     |
 |______________|______________|______________|______________|

 The next element that is freed will contain the offset from avail and avail
  will now point to the freed element.
 *pobj = avail - pobj
 avail = pobj

       start                                        end
       avail                                         |
         |                                           |
        \|/                                         \|/
  ___________________________________________________________
 |     -2 *     |              |              |              |
 | sizeof(type) |     data     | guard value  |     data     |
 |______________|______________|______________|______________|

 Now after the first available element is used, avail will point to the next
  free element.
 avail = avail + *avail

       start                                        end
         |                           avail           |
         |                             |             |
        \|/                           \|/           \|/
  ___________________________________________________________
 |              |              |              |              |
 |   new data   |     data     | guard value  |     data     |
 |______________|______________|______________|______________|

```

The macro `MIN_ELEMENT_SIZE` determines the minimum size of memory used to store the offset values, therefore decides the maximum size of the memory pool. The size of chunks to be used should not be smaller than `MIN_ELEMENT_SIZE`. It defaults to 1 but can be defined before including the header `memblock.h`. Accepted values are 1, 2, 4 and 8.

```c
#define MIN_ELEMENT_SIZE 2
#include <memblock.h>
```
