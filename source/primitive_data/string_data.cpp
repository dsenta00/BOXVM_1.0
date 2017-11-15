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

#include <ORM/orm.h>
#include <error_log.h>
#include <memory.h>
#include <virtual_memory.h>
#include <iostream>
#include <utility>
#include "primitive_data/string_data.h"

/**
 * The constructor.
 *
 * @param id
 * @param value
 */
string_data::string_data(std::string id, const void *value) : primitive_data::primitive_data(
    std::move(id), DATA_TYPE_STRING, value
)
{
    if (!value)
    {
        this->default_value();
    }
}

/**
 * The constructor.
 *
 * @param id
 * @param data
 */
string_data::string_data(std::string id, string_data &data) : primitive_data::primitive_data(
    std::move(id), data, true
)
{
}

/**
 * @param id
 * @param type
 * @param value
 * @return
 */
string_data *
string_data::create(std::string id, const void *value)
{
    return (string_data *) orm::create((object *) new string_data(std::move(id), value));
}

/**
 * @param id
 * @param type
 * @param value
 * @return
 */
string_data *
string_data::create(std::string id, string_data &data)
{
    return (string_data *) orm::create((object *) new string_data(std::move(id), data));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
string_data::to_bool()
{
    bool value = false;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = !this->get_string().empty();

    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
wchar_t
string_data::to_char()
{
    wchar_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = mem->get_element<wchar_t>();

    return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
string_data::to_int()
{
    int32_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    swscanf(mem->get_pointer<const wchar_t *>(),
            DATA_TYPE_FORMAT[DATA_TYPE_INT],
            &value);

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
string_data::to_float()
{
    double value = 0.0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    swscanf(mem->get_pointer<const wchar_t *>(),
            DATA_TYPE_FORMAT[DATA_TYPE_FLOAT],
            &value);

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
primitive_data &
string_data::to_string()
{
    std::wstring string = this->get_string();

    return *string_data::create(this->id.append(" as string"), string.c_str());
}

/**
 * Set data to default value.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::default_value()
{
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    memset(mem->get_pointer<void *>(), 0, mem->get_size());

    return true;
}

/**
 * Operator =. Assign value from memory address.
 *
 * @param data - the data memory address.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator=(const void *data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    const auto *str_temp = (const wchar_t *) data;
    auto str_size = static_cast<uint32_t>((wcslen(str_temp) + 1) * sizeof(wchar_t));

    if (mem->get_size() < str_size)
    {
        memory *new_mem = this->vm->alloc(str_size);

        object::master_relationship_remove_object("primitive_data_memory", (object *) mem);
        object::master_relationship_add_object("primitive_data_memory", (object *) new_mem);

        this->vm->free(mem);
        mem = new_mem;
    }

    wcsncpy(mem->get_pointer<wchar_t *>(), str_temp, str_size);

    return true;
}

/**
 * Operator =. Assign value from another data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator=(primitive_data &data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->get_element<wchar_t>() = 0;
    return (*this) += (data);
}

/**
 * Operator &=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator&=(primitive_data &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);

    return false;
}

/**
 * Operator |=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator|=(primitive_data &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);

    return false;
}

/**
 * Operator ^=
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator^=(primitive_data &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);

    return false;
}

/**
 * Operator +=. Do operation add if primitive data type.
 * On string data type append data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator+=(primitive_data &data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::wstring string = data.get_string();
    auto request_size = static_cast<uint32_t>(wcslen(mem->get_pointer<const wchar_t *>()) * sizeof(wchar_t) +
                                              (wcslen(string.c_str()) + 1) * sizeof(wchar_t));

    if (mem->get_size() < request_size)
    {
        memory *new_mem = vm->realloc(mem, request_size);

        if (new_mem->get_size() != request_size)
        {
            /* Something really bad happened. */
            return false;
        }

        if (new_mem != mem)
        {
            /*
             * new_mem is different than mem,
             * switch relations.
             */
            object::master_relationship_remove_object("primitive_data_memory", (object *) mem);
            object::master_relationship_add_object("primitive_data_memory", (object *) new_mem);

            mem = new_mem;
        }
    }

    wcscat((wchar_t *) mem->get_address(), string.c_str());
    return true;

}

/**
 * Operator -=. Do subtract operation.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator-=(primitive_data &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);

    return false;
}

/**
 * Operator *=. Multiply values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator*=(primitive_data &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);

    return false;
}

/**
 * Operator /=. Divide values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator/=(primitive_data &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);

    return false;
}

/**
 * Operator %=. Do modulus between values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator%=(primitive_data &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);

    return false;
}

/**
 * Operator ++. Increment value.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator++()
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INCREMENTING_STRING);

    return false;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::operator--()
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DECREMENTING_STRING);

    return false;
}

/**
 * Operator ==. Check values equality.
 *
 * @param data - the data.
 *
 * @return true if equal, otherwise return false.
 */
bool
string_data::operator==(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
    {
        return wcscmp(mem->get_pointer<const wchar_t *>(),
                      (const wchar_t *) data.get_address()) == 0;
    }
    else
    {
        std::wstring string = data.get_string();
        return string == mem->get_pointer<const wchar_t *>();
    }
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
string_data::operator!=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
    {
        return wcscmp(mem->get_pointer<const wchar_t *>(),
                      (const wchar_t *) data.get_address()) != 0;
    }
    else
    {
        std::wstring string = data.get_string();

        return string != mem->get_pointer<const wchar_t *>();
    }
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
string_data::operator>(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
    {
        return wcscmp(mem->get_pointer<const wchar_t *>(),
                      (const wchar_t *) data.get_address()) > 0;
    }
    else
    {
        std::wstring string = data.get_string();
        return string.compare(mem->get_pointer<const wchar_t *>()) < 0;
    }
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
string_data::operator<(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
    {
        return wcscmp(mem->get_pointer<const wchar_t *>(),
                      (const wchar_t *) data.get_address()) < 0;
    }
    else
    {
        std::wstring string = data.get_string();
        return string.compare(mem->get_pointer<const wchar_t *>()) > 0;
    }
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
string_data::operator>=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
    {
        return wcscmp(mem->get_pointer<const wchar_t *>(),
                      (const wchar_t *) data.get_address()) >= 0;
    }
    else
    {
        std::wstring string = data.get_string();
        return string.compare(mem->get_pointer<const wchar_t *>()) <= 0;
    }
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
string_data::operator<=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
    {
        return wcscmp(mem->get_pointer<const wchar_t *>(),
                      (const wchar_t *) data.get_address()) <= 0;
    }
    else
    {
        std::wstring string = data.get_string();
        return string.compare(mem->get_pointer<const wchar_t *>()) >= 0;
    }
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::print()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::wcout << mem->get_pointer<wchar_t *>();

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::println()
{
    if (this->print())
    {
        std::cout << std::endl;
        return true;
    }

    return false;
}

/**
 * Insert value from console terminal into data.
 *
 * @return true if success, otherwise return false.
 */
bool
string_data::scan()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::wcin >> mem->get_pointer<wchar_t *>();

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::wstring
string_data::get_string()
{
    std::wstring str;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return str;
    }

    str.assign(mem->get_pointer<const wchar_t *>());

    return str;
}