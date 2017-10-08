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

#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "orm_fw.h"
#include "relationship_type.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>

typedef enum {
  FOREACH_CONTINUE,
  FOREACH_IT2_REMOVED,
  FOREACH_IT1_REMOVED
} foreach_result;

/**
 * The relationship class.
 * Contains and manages all entity objects in relationship.
 */
class relationship {
public:
  relationship(std::string relationship_name, relationship_type type);
  std::string &get_name();
  relationship_type get_type();
  entity *find(const std::function<bool(entity *)> &func);
  void sort(const std::function<bool(entity *, entity *)> &func);
  void for_each(const std::function<foreach_result(entity *, entity *)> &func);
  void for_each(const std::function<void(entity *)> &func);
  void add_entity(entity *e);
  void remove_entity(entity *e);
  std::vector<entity *> &get_entities();
  entity *front();
  entity *back();
  uint32_t num_of_entities();
protected:
  std::string relationship_name;
  relationship_type type;
  std::vector<entity *> entities;
};

#endif // RELATIONSHIP_H
