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

#pragma once

#include "Primitive.h"

/**
 * Character data type.
 */
class Char : public Primitive {
public:
    explicit Char(const void *value = nullptr);
    Char(Char &data);
    static Char *create(const void *value = nullptr);
    static Char *create(Char &data);

    bool toBool() override;
    wchar_t toChar() override;
    int32_t toInt() override;
    double toFloat() override;
    String &toString() override;

    eObjectType getObjectType() override;
    bool isReference() override;
    bool defaultValue() override;
    bool operator=(const void *data) override;
    bool operator=(Value &data) override;
    bool operator&=(Value &data) override;
    bool operator|=(Value &data) override;
    bool operator^=(Value &data) override;
    bool operator+=(Value &data) override;
    bool operator-=(Value &data) override;
    bool operator*=(Value &data) override;
    bool operator/=(Value &data) override;
    bool operator%=(Value &data) override;
    bool operator++() override;
    bool operator--() override;
    bool operator==(Value &data) override;
    bool operator!=(Value &data) override;
    bool operator>(Value &data) override;
    bool operator<(Value &data) override;
    bool operator>=(Value &data) override;
    bool operator<=(Value &data) override;

    bool print() override;
    bool println() override;
    bool scan() override;

    std::wstring getString() override;
};