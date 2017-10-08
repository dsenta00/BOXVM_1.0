/*
 * Copyright 2017 Duje Senta, Tomislav Radanovic
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef BOX_MEMORY_CHUNK_IF_H
#define BOX_MEMORY_CHUNK_IF_H

#include "ORM/entity.h"
#include "box_fw.h"
#include <cstdint>
#include <cstdlib>
#include <functional>

class memory_chunk_if : public entity {
public:
  memory_chunk_if();
  void free_memory_add(uintptr_t address, uint32_t size);

  void free_memory_remove(memory *mem);
  memory *free_memory_find(std::function<bool(memory *)> foo);
  memory *free_memory_front();
  uint32_t free_memory_num();
  void free_memory_delete_all();
  void free_memory_union();
  memory *reserved_memory_add(uintptr_t address, uint32_t size);

  void reserved_memory_remove(memory *mem);
  memory *reserved_memory_front();
  memory *reserved_memory_back();
  uint32_t reserved_memory_num();
  void reserved_memory_sort();
};

#endif // BOX_MEMORY_CHUNK_IF_H
