
#ifndef _MEMBLOCK_H_
#define _MEMBLOCK_H_

#define byte_ptr_type unsigned char*

struct static_memblock {
  unsigned char *avail;
};

#define init_mem(name, mem, size) \
  name.avail = (byte_ptr_type)mem; \
  *(signed char*)(mem + size - 1) = 0xff; \
  for (signed char i = 0; i < size - 1; i++) \
    *(signed char*)(mem + i) = sizeof(mem[0]);

#define use_mem(type, block) ({ \
  type *__obj = 0; \
  if (block.avail) { \
    __obj = (type *)block.avail; \
    if (*(signed char*)block.avail == -1) \
      block.avail = 0; \
    else \
      block.avail += *(signed char*)block.avail; \
  } \
  __obj; \
})

#define free_mem(pobj, block) \
  if (pobj) { \
    *(signed char*)pobj = (!block.avail) ? \
      0xff : (signed char)(block.avail - (byte_ptr_type)pobj); \
    block.avail = (byte_ptr_type)pobj; \
  }

#endif /* _MEMBLOCK_H_ */
