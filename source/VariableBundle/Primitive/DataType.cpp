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

#include <VariableBundle/Primitive/DataType.h>
#include <regex>

/**
 * Get data type from token.
 *
 * @param str
 * @return
 */
eObjectType
DataType::getFromToken(std::wstring str)
{
    if (str == L"bool")
    {
        return OBJECT_TYPE_BOOL;
    }
    else if (str == L"char")
    {
        return OBJECT_TYPE_CHAR;
    }
    else if (str == L"int")
    {
        return OBJECT_TYPE_INT;
    }
    else if (str == L"float")
    {
        return OBJECT_TYPE_FLOAT;
    }
    else if (str == L"string")
    {
        return OBJECT_TYPE_STRING;
    }
    else
    {
        return OBJECT_TYPE_NULL;
    }
}

/**
 * Detect data_type.
 *
 * @param sample
 * @return
 */
eObjectType
DataType::detect(std::wstring sample)
{
    if (sample.empty())
    {
        return OBJECT_TYPE_NULL;
    }

    if (std::regex_match(sample, std::wregex(L"([-+]?[0-9]+)")))
    {
        return OBJECT_TYPE_INT;
    }
    else if (std::regex_match(sample, std::wregex(L"[-+]?[0-9]*\\.?[0-9]*")))
    {
        return OBJECT_TYPE_FLOAT;
    }
    else if (std::regex_match(sample, std::wregex(L"\'?.\'")))
    {
        return OBJECT_TYPE_CHAR;
    }
    else if (std::regex_match(sample, std::wregex(L"\"(.*)\"")) or std::regex_match(sample, std::wregex(L"\'(.*)+\'")))
    {
        return OBJECT_TYPE_STRING;
    }

    std::wstring sample_temp = sample;
    std::for_each(sample_temp.begin(), sample_temp.end(), [&](wchar_t &c) { c = std::tolower(c); });

    if (std::regex_match(sample_temp, std::wregex(L"true|false")))
    {
        return OBJECT_TYPE_BOOL;
    }
    else if (std::regex_match(sample_temp, std::wregex(L"null")))
    {
        return OBJECT_TYPE_NULL;
    }
    else
    {
        return OBJECT_TYPE_NULL;
    }
}

/**
 * Clean constant format.
 *
 * @param sample
 * @param type
 * @return
 */
std::wstring
DataType::cleanConstantFormat(std::wstring &sample, eObjectType type)
{
    switch (type)
    {
        case OBJECT_TYPE_INT:
        case OBJECT_TYPE_FLOAT:
            /*
             * nothing to do.
             */
            break;
        case OBJECT_TYPE_CHAR:
        case OBJECT_TYPE_STRING:
        {
            sample.erase(sample.begin());
            sample.erase(sample.end() - 1);
            break;
        }
        default:
            break;
    }

    return sample;
}
