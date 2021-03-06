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

#include "VariableBundle/File/FileMode.h"

/**
 * Check if can read in this file mode.
 *
 * @param mode
 * @return
 */
bool
FileMode::canRead(eFileMode mode)
{
    return (mode & FILE_MODE_READ) == FILE_MODE_READ;
}

/**
 * Check if can write in this mode.
 *
 * @param mode
 * @return
 */
bool
FileMode::canWrite(eFileMode mode)
{
    return (mode & FILE_MODE_WRITE) == FILE_MODE_WRITE;
}

/**
 * Check if mode is valid for opening file.
 *
 * @param mode
 * @return
 */
bool
FileMode::isValid(eFileMode mode)
{
    switch (mode)
    {
        case FILE_MODE_READ:
        case FILE_MODE_WRITE:
        case FILE_MODE_APPEND:
            return true;
        default:
        case FILE_MODE_NOT_OPEN:
            return false;
    }
}