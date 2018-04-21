/*
 * Copyright 2017 Duje Senta
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

#pragma once

#include "ObjectRepository.h"
#include "eObjectType.h"
#include <string>
#include <functional>

/**
 * ORM interface.
 */
namespace ORM {
    ObjectRepository *findObjectRepository(eObjectType type);
    void addObjectRepository(eObjectType type);
    Object *create(Object *o);
    void changeId(Object *o, std::string new_id);
    void destroy(Object *o);
    void sweep();
    Object *select(eObjectType type, std::function<bool(Object *)> where);
    Object *select(eObjectType type, std::string id);
    Object *getFirst(eObjectType type);
    void removeObjectRepository(eObjectType type);
    void removeAllRepositories();
}

#define ORM_DESTROY(__OBJ__) \
  ORM::destroy((Object *)(__OBJ__))