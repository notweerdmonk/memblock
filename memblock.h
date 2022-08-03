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

#include <stdint.h>

/* Define MEMBLOCK_MIN_ELEMENT_SIZE as per need */
#ifndef MEMBLOCK_MIN_ELEMENT_SIZE
#define MEMBLOCK_MIN_ELEMENT_SIZE 1
#endif

typedef uint8_t* byte_ptr_type;

#if MEMBLOCK_MIN_ELEMENT_SIZE == 1

typedef int8_t arithmetic_type;
typedef uint8_t guard_value_type;
const guard_value_type MEMBLOCK_GUARD_VALUE = 0x80;
enum { MAX_MEMORY_SIZE = 0x7f };

#elif MEMBLOCK_MIN_ELEMENT_SIZE == 2

typedef int16_t arithmetic_type;
typedef uint16_t guard_value_type;
const guard_value_type MEMBLOCK_GUARD_VALUE = 0x8000;
enum { MAX_MEMORY_SIZE = 0x7fff };

#elif MEMBLOCK_MIN_ELEMENT_SIZE == 4

typedef int32_t arithmetic_type;
typedef uint32_t guard_value_type;
const guard_value_type MEMBLOCK_GUARD_VALUE = 0x80000000;
enum { MAX_MEMORY_SIZE = 0x7fffffff };

#elif MEMBLOCK_MIN_ELEMENT_SIZE == 8

typedef int64_t arithmetic_type;
typedef uint64_t guard_value_type;
const guard_value_type MEMBLOCK_GUARD_VALUE = 0x8000000000000000;
enum { MAX_MEMORY_SIZE = 0x7fffffffffffffff };

#else
#error Invalid MEMBLOCK_MIN_ELEMENT_SIZE
#endif

struct memblock {
  byte_ptr_type avail;
  byte_ptr_type start;
  byte_ptr_type end;
};

/**
 * @brief This macro does bounds checking while using or discarding chunks.
 * @param ptr The pointer to check.
 * @param start The start pointer of a memblock.
 * @param end The end pointer of a memblock.
 */
#define _chk_bounds(ptr, start, end) \
  (((byte_ptr_type)ptr >= start) && ((byte_ptr_type)ptr <= end))

/**
 * @brief init_mem initializes the memory pool.
 *
 * It sets the start and end pointers as per the size and number of chunks
 * requested and the avail pointer is set to point to the first chunk. A guard
 * value is written at the last chunk. A chain of links is created from the
 * starting chunk by writing at each chunk, the offset to the next chunk.
 *
 * @param name Name of struct memblock.
 * @param type Type of chunks.
 * @param mem Block of allocated memory.
 * @param size Number of chunks to be used from memory block.
 *
 * @return A pointer to the first available memory location.
 */
#define init_mem(name, type, mem, size) ({ \
  if (size <= MAX_MEMORY_SIZE) { \
    name.avail = name.start = (byte_ptr_type)mem; \
    name.end = (byte_ptr_type)mem + (size - 1) * sizeof(type); \
    *(guard_value_type*)name.end = MEMBLOCK_GUARD_VALUE; \
    for (arithmetic_type i = 0; i < size - 1; i++) \
      *(arithmetic_type*)((byte_ptr_type)mem + i * sizeof(type)) = sizeof(type); \
  } else { \
    name.avail = 0; \
  } \
  name.avail; \
})

/**
 * @brief use_mem provides a pointer to a chunk of memory.
 *
 * The avail pointer is set to point to the next available chunk. It is set to
 * zero when the memory pool has been exhausted so that a null pointer is
 * provided for subsequent requests.
 *
 * @param type Type of chunk.
 * @param block Name of struct memblock.
 *
 * @return A pointer of requested type pointing to a chunk of memory or zero if
 * the memory pool has been exhausted.
 */
#define use_mem(type, block) ({ \
  type *__obj = 0; \
  if (block.avail && _chk_bounds(block.avail, block.start, block.end)) { \
    __obj = (type *)block.avail; \
    block.avail = (*(guard_value_type*)block.avail == MEMBLOCK_GUARD_VALUE) ? \
    0 : block.avail + *(arithmetic_type*)block.avail; \
  } \
  __obj; \
})

/**
 * @brief free_mem relinquishes a chunk of memory.
 *
 * If the memory pool has been exhaused the guard value is written at the first
 * chunk that is freed, otherwise the offset to the previous available chunk is
 * written. avail pointer is set to point to the recently freed chunk.
 *
 * @param pobj Pointer to memory to be freed.
 * @param block Name of struct memblock.
 */
#define free_mem(pobj, block) \
  if (pobj && _chk_bounds(pobj, block.start, block.end)) { \
    if (!block.avail) \
      *(guard_value_type*)pobj = MEMBLOCK_GUARD_VALUE; \
    else \
      *(arithmetic_type*)pobj = \
        (arithmetic_type)(block.avail - (byte_ptr_type)pobj); \
    block.avail = (byte_ptr_type)pobj; \
  }

#endif /* _MEMBLOCK_H_ */
