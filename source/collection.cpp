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

#include "collection.h"
#include "primitive_data.h"
#include "error_log.h"
#include "ORM/relationship.h"
#include "ORM/orm.h"
#include "virtual_memory.h"

/**
 * The constructor.
 *
 * @param type - data type.
 * @param size - array size in members.
 */
collection::collection(std::string id, collection *c) : entity::entity("collection", id)
{
    this->master_relationship_add("collection", ONE_TO_MANY);

    if (c == nullptr)
    {
        return;
    }

    (*this) += (entity *) c;
}

/**
 * Get number of data.
 *
 * @return number of data.
 */
uint32_t
collection::get_number_of()
{
    return static_cast<uint32_t>(this->data_cache.size());
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
entity *
collection::operator[](uint32_t index)
{
    return (*this)[std::to_string(index)];
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
entity *
collection::operator[](std::string index)
{
    return this->data_cache[index];
}

/**
 * @brief collection::insert
 * @param index
 * @param e
 */
void
collection::insert(std::string index, entity *e)
{
    this->remove_data(index);
    this->insert_data(index, e);
}

/**
 * @brief collection::insert
 * @param index
 * @param e
 */
void
collection::insert(uint32_t index, entity *e)
{
    this->insert(std::to_string(index), e);
}

/**
 * Operator +=. Copy and append data to this array.
 *
 * @param data - the data.
 * @return true if operation success, otherwise return false.
 */
bool
collection::operator+=(entity *e)
{
    this->insert_data(std::to_string(this->get_number_of()), e);
    return true;
}

/**
 * Convert collection into string.
 *
 * @return the data string.
 */
primitive_data &
collection::to_string()
{
    primitive_data &str = *primitive_data::create(this->id.append(" as string"),
                                                  DATA_TYPE_STRING);

    relationship *r = this->master_relationship_get("collection");

    if (!r)
    {
        return str;
    }

    char ch = ' ';
    primitive_data &separator_char = *primitive_data::create("<<temp_char>>",
                                                             DATA_TYPE_CHAR,
                                                             (const void *) &ch);


    r->for_each([&](entity *e) {
        if (e->get_entity_type() == "primitive_data")
        {
            auto *data = (primitive_data *) e;
            str += *data;

            if (!ERROR_LOG_IS_EMPTY)
            {
                str.default_value();
                return;
            }

            if (data != r->back())
            {
                str += separator_char;
            }
        }
        else if (e->get_entity_type() == "collection")
        {
            auto *data = (collection *) e;
            str += data->to_string();

            if (!ERROR_LOG_IS_EMPTY)
            {
                str.default_value();
                return;
            }

            if (data != r->back())
            {
                str += separator_char;
            }
        }
    });

    virtual_memory *vm = (virtual_memory *) orm::get_first("virtual_memory");
    vm->free(separator_char.get_memory());
    orm::destroy(&separator_char);

    return str;
}

/**
 * Clear collection.
 */
void
collection::clear()
{
    relationship *r = this->master_relationship_get("collection");

    while (r->num_of_entities())
    {
        this->remove_data(r->front());
    }

    this->data_cache.clear();
}

/**
 * Remove data from index.
 *
 * @param index
 */
void
collection::remove_data(std::string index)
{
    this->remove_data(this->data_cache[index]);
}

/**
 * Remove data.
 *
 * @param e
 */
void
collection::remove_data(entity *e)
{
    if (!e)
    {
        return;
    }

    this->master_relationship_remove_entity("collection", e);
    this->data_cache.erase(e->get_id());
}

/**
 * @brief collection::insert_data
 * @param index
 * @param e
 */
void
collection::insert_data(std::string index, entity *e)
{
    if (e == nullptr)
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADDING_NULL_DATA);
        return;
    }

    if (e->get_entity_type() == "primitive_data")
    {
        /*
         * Data is not a reference. Create a new data.
         */
        entity *new_data = primitive_data::create(index, *(primitive_data *) e);
        e = new_data;
    }

    this->master_relationship_add_entity("collection", e);
    this->data_cache[index] = e;
}

/**
 * @brief collection::~collection()
 */
collection::~collection()
{
    this->clear();
}

/**
 * @brief create
 * @param id
 * @param c
 * @return
 */
collection *
collection::create(std::string id, collection *c)
{
    return (collection *) orm::create((entity *) new collection(std::move(id), c));
}