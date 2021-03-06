/*
 * Copyright 2018 Duje Senta
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

#include <utility>
#include <algorithm>

#include <ORM/Relationship.h>
#include <ORM/Object.h>
#include <ErrorBundle/ErrorLog.h>

/**
 * The constructor.
 *
 * @param relationshipName - relationship name.
 * @param type - relationship type.
 */
Relationship::Relationship(std::string relationshipName, eRelationshipType type)
{
    this->name = std::move(relationshipName);
    this->type = type;
}

/**
 * Get relationship name.
 *
 * @return relationship name.
 */
std::string &
Relationship::getName()
{
    return this->name;
}

/**
 * Get relationship type.
 *
 * @return relationship type.
 */
eRelationshipType
Relationship::getType()
{
    return this->type;
}

/**
 * Find object.
 *
 * @param func - function rule.
 * @return object if found, otherwise return NULL.
 */
Object *
Relationship::find(const std::function<bool(Object *)> &func)
{
    auto it = std::find_if(this->begin(), this->end(), func);

    return (it != this->end()) ? *it : nullptr;
}

/**
 * Sort entities.
 *
 * @param func - function to sort.
 */
void
Relationship::sort(const std::function<bool(Object *, Object *)> &func)
{
    std::sort(this->begin(), this->end(), func);
}

/**
 * foreach two neighbour iterators.
 *
 * @param func - function to handle current iterators.
 */
void
Relationship::forEach(const std::function<eForEachResult(Object *, Object *)> &func)
{
    for (auto it1 = this->begin(); it1 != this->end(); it1++)
    {
        auto it2 = it1;
        it2++;

        while (it2 != this->end())
        {
            eForEachResult result = func(*it1, *it2);

            switch (result)
            {
                case FOREACH_CONTINUE:
                    it1++;
                    it2++;
                    break;
                case FOREACH_IT2_REMOVED:
                    it2 = it1;
                    it2++;
                    break;
                case FOREACH_IT1_REMOVED:
                    it1 = it2;
                    it2++;
                    break;
                default:
                    return;
            }
        }
    }
}

/**
 * Add object to this relationship.
 *
 * @param o - the object.
 */
void
Relationship::addObject(Object *o)
{
    if (this->type == ONE_TO_ONE)
    {
        if (!this->empty())
        {
            ERROR_LOG_ADD(ERROR_RELATIONSHIP_ADDING_MORE_THAN_ONE);
            return;
        }
    }

    o->setMarked(false);
    this->push_back(o);
}

/**
 * Remove object from this relationship.
 *
 * @param o - the object.
 */
void
Relationship::removeObject(Object *o)
{
    if (!o)
    {
        return;
    }

    for (auto it = this->begin(); it != this->end(); it++)
    {
        if (*it == o)
        {
            this->erase(it);
            break;
        }
    }
}

/**
 * @inherit
 */
Object *
Relationship::front()
{
    return ObjVector::empty() ? nullptr : ObjVector::front();
}

/**
 * @inherit
 */
Object *
Relationship::back()
{
    return ObjVector::empty() ? nullptr : ObjVector::back();
}

Object *
Relationship::find(std::string id)
{
    for (Object *o : (*this))
    {
        if (o->getId() == id)
        {
            return o;
        }
    }

    return nullptr;
}
