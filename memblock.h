/*
  MIT License
  
  Copyright (c) 2022 notweerdmonk
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef _MEMBLOCK_H_
#define _MEMBLOCK_H_

/* Define MIN_ELEMENT_SIZE as per need */
#ifndef MIN_ELEMENT_SIZE
#define MIN_ELEMENT_SIZE 1
#endif

#define byte_ptr_type unsigned char*

#if MIN_ELEMENT_SIZE == 1

#define arithmetic_type  signed char
#define MAX_MEMORY_SIZE  0x7f
#define guard_value_type unsigned char
#define GUARD_VALUE      0x80

#elif MIN_ELEMENT_SIZE == 2

#define arithmetic_type  signed short
#define MAX_MEMORY_SIZE  0x7fff
#define guard_value_type unsigned short
#define GUARD_VALUE      0x8000

#elif MIN_ELEMENT_SIZE == 4

#define arithmetic_type  signed int
#define MAX_MEMORY_SIZE  0x7fffffff
#define guard_value_type unsigned int
#define GUARD_VALUE      0x80000000

#elif MIN_ELEMENT_SIZE == 8

#define arithmetic_type  signed long
#define MAX_MEMORY_SIZE  0x7fffffffffffffff
#define guard_value_type unsigned long
#define GUARD_VALUE      0x8000000000000000

#else
#error Invalid MIN_ELEMENT_SIZE
#endif

struct memblock {
  byte_ptr_type avail;
  byte_ptr_type start;
  byte_ptr_type end;
};

#define _chk_bounds(ptr, start, end) \
  (((byte_ptr_type)ptr >= start) && ((byte_ptr_type)ptr <= end))

#define init_mem(name, type, mem, size) ({ \
  if (size <= MAX_MEMORY_SIZE) { \
    name.avail = name.start = (byte_ptr_type)mem; \
    name.end = (byte_ptr_type)mem + (size - 1) * sizeof(type); \
    *(guard_value_type*)name.end = GUARD_VALUE; \
    for (arithmetic_type i = 0; i < size - 1; i++) \
      *((arithmetic_type*)mem + i * sizeof(type)) = sizeof(type); \
  } else { \
    name.avail = 0; \
  } \
  name.avail; \
})

#define use_mem(type, block) ({ \
  type *__obj = 0; \
  if (block.avail && _chk_bounds(block.avail, block.start, block.end)) { \
    __obj = (type *)block.avail; \
    block.avail = (*(guard_value_type*)block.avail == GUARD_VALUE) ? \
    0 : block.avail + *(arithmetic_type*)block.avail; \
  } \
  __obj; \
})

#define free_mem(pobj, block) \
  if (pobj && _chk_bounds(pobj, block.start, block.end)) { \
    if (!block.avail) \
      *(guard_value_type*)pobj = GUARD_VALUE; \
    else \
      *(arithmetic_type*)pobj = \
        (arithmetic_type)(block.avail - (byte_ptr_type)pobj); \
    block.avail = (byte_ptr_type)pobj; \
  }

#endif /* _MEMBLOCK_H_ */
