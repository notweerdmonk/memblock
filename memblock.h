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

#define byte_ptr_type unsigned char*

struct memblock {
  unsigned char *avail;
};

#define init_mem(name, type, mem, size) \
  name.avail = (byte_ptr_type)mem; \
  *(signed char*)(mem + (size - 1) * sizeof(type)) = 0xff; \
  for (signed char i = 0; i < size - 1; i++) \
    *(signed char*)(mem + i * sizeof(type)) = sizeof(type);

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
