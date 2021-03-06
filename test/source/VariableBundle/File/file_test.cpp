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

#include "ORM/ORM.h"
#include "ErrorBundle/ErrorLog.h"
#include <VariableBundle/Collection/Collection.h>
#include <VariableBundle/Primitive/String.h>
#include <VariableBundle/Primitive/Float.h>
#include <VariableBundle/Primitive/Int.h>
#include "VariableBundle/file/File.h"
#include <cstdio>
#include <codecvt>
#include <locale>
#include "../../../test_assert.h"

#define TEST_TEXT (L"1234 miljenko _+_")

std::string convert_utf8_to_ascii(std::wstring ws)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(ws);
}

/**
 * create_file
 * @param file_name
 * @param text
 */
static void
create_file(const char *file_name, const wchar_t *text)
{
    FILE *fp = nullptr;

    fp = fopen(file_name, "w+");

    if (!fp)
    {
        perror("Problem!... skipping test");
        return;
    }

    if (wcslen(text) > 0)
    {
        std::string str = convert_utf8_to_ascii(text);
        fwrite(str.c_str(),
               sizeof(int8_t),
               str.size(),
               fp);
    }

    fclose(fp);
}

/**
 * compare_file_content
 * @param fileName
 * @param text
 * @return
 */
static bool
compare_file_content(const char *fileName, const char *text)
{
    FILE *fp = nullptr;

    fp = fopen(fileName, "r");

    if (!fp)
    {
        perror("Problem");
        return false;
    }

    uint64_t size = strlen(text);
    auto *str = (char *) calloc(size + 1, sizeof(int8_t));

    if (!str)
    {
        perror("Problem!");
        return false;
    }

    fread(str, sizeof(int8_t), size + 1, fp);
    fclose(fp);

    bool result = (strcmp(str, text) == 0);
    free(str);
    return result;
}

/**
 * file_test_read
 */
static void
file_test_read()
{
    /*
     * Negative test case
     */
    File *f = File::create(FILE_MODE_READ, "miljenkoooooo");

    ASSERT_ERROR(ERROR_FILE_UNKNOWN_FILE);
    ASSERT_NOT_NULL(f);
    ASSERT_FALSE(f->isOpened(), "File should not be opened!");
    ERROR_LOG_CLEAR;
    ORM_DESTROY(f);

    /*
     * Positive test case
     */
    const char *file_name = "test_star.txt";
    create_file("test_star.txt", TEST_TEXT);
    f = File::create( FILE_MODE_READ, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->isOpened(), "File should be opened!");

    auto *str = f->readAll();
    ASSERT_OK;
    ASSERT_NOT_NULL(str);
    ASSERT_TRUE(wcscmp((const wchar_t *) str->getAddress(), TEST_TEXT) == 0,
                "Should be equal! \"%ls\"",
                (const wchar_t *) str->getAddress());
    ORM_DESTROY(str);
    /* try again */
    str = f->readAll();
    ASSERT_OK;
    ASSERT_NOT_NULL(str);
    ASSERT_TRUE(wcscmp((const wchar_t *) str->getAddress(), TEST_TEXT) == 0,
                "Should be equal! \"%ls\"",
                (const wchar_t *) str->getAddress());
    ORM_DESTROY(str);

    f->close();

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_FALSE(f->isOpened(), "File should not be opened!");

    ORM_DESTROY(f);
    remove(file_name);
}

/**
 * file_test_write1
 */
static void
file_test_write1()
{
    const char *file_name = "test_star_write.txt";
    File *f = File::create(FILE_MODE_WRITE, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->isOpened(), "File should be opened!");
    ASSERT_EQUALS(f->get_mode(), FILE_MODE_WRITE);

    Primitive *str = String::create(L"Miljenko 123");
    f->write(str);
    ASSERT_EQUALS(f->get_mode(), FILE_MODE_WRITE);
    ASSERT_OK;

    f->close();
    ASSERT_OK;
    ASSERT_EQUALS(f->get_mode(), FILE_MODE_NOT_OPEN);
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 123"), "Should be equal!");

    ORM_DESTROY(f);
    ORM_DESTROY(str);
    remove(file_name);
}

/**
 * file_test_write2
 */
static void
file_test_write2()
{
    const char *file_name = "test_star_write.txt";
    File *f = File::create(FILE_MODE_WRITE, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->isOpened(), "File should be opened!");

    /* fill Collection */
    Collection *c = Collection::create();
    int var1 = 42;
    *c += *Int::create(&var1);
    double var2 = 41.0f;
    *c += *Float::create(&var2);
    const wchar_t *var3 = L"40";
    *c += *String::create(var3);
    /* fill Collection end */
    f->write(c);
    ASSERT_OK;

    ASSERT_OK;
    ORM_DESTROY(f);
    ORM_DESTROY(c);
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "42 41.000000 40"), "Should be equal!");

    remove(file_name);
}

/**
 * file_test_read_empty_file
 */
static void
file_test_read_empty_file()
{
    const char *file_name = "test_star.txt";
    create_file("test_star.txt", L"");
    File *f = File::create(FILE_MODE_READ, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->isOpened(), "File should be opened!");

    auto *str = f->readAll();
    ASSERT_NOT_NULL(str);
    ASSERT_TRUE(strcmp((const char *) str->getAddress(), "") == 0,
                "Should be equal! %s",
                (const char *) str->getAddress());
    ORM_DESTROY(str);
    /* try again */
    str = f->readAll();
    ASSERT_NOT_NULL(str);
    ASSERT_TRUE(strcmp((const char *) str->getAddress(), "") == 0,
                "Should be equal! %s",
                (const char *) str->getAddress());
    ORM_DESTROY(str);

    f->close();

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_FALSE(f->isOpened(), "File should not be opened!");

    ORM_DESTROY(f);

    remove(file_name);
}

/**
 * file_test_append
 */
static void
file_test_append()
{
    const char *file_name = "test_star_write.txt";
    File *f = File::create(FILE_MODE_APPEND, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->isOpened(), "File should be opened!");

    auto *str = String::create(L"Miljenko 123");
    f->write(str);
    ASSERT_OK;

    f->close();
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 123"), "Should be equal!");

    ORM_DESTROY(f);
    ORM_DESTROY(str);

    f = File::create(FILE_MODE_APPEND, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->isOpened(), "File should be opened!");

    str = String::create(L"74, 345");
    f->write(str);
    ASSERT_OK;

    f->close();
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 12374, 345"), "Should be equal!");
    remove(file_name);
}

/**
 * Test reopen.
 */
static void
file_test_reopen()
{
    const char *file_name = "test_star_write.txt";
    File *f = File::create();
    ASSERT_OK;
    ASSERT_FALSE(f->isOpened(), "File should not be open!");

    f->open(FILE_MODE_WRITE, file_name);
    ASSERT_OK;
    ASSERT_TRUE(f->isOpened(), "File should be open!");

    auto *str = String::create(L"Miljenko 123");
    f->write(str);
    ASSERT_OK;
    ORM_DESTROY(str);

    f->open(FILE_MODE_APPEND, file_name);
    str = String::create(L"74, 345");
    f->write(str);
    ASSERT_OK;
    ORM_DESTROY(str);

    f->open(FILE_MODE_READ, file_name);
    str = f->readAll();
    ASSERT_TRUE(wcscmp((const wchar_t *) str->getAddress(),
                       L"Miljenko 12374, 345") == 0, "should be equal");

    f->close();
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 12374, 345"), "Should be equal!");
    remove(file_name);
}

/**
 * test file.
 */
void
file_test()
{
    //RUN_TEST(file_test_read());
    RUN_TEST(file_test_read_empty_file());
    RUN_TEST(file_test_write1());
    //RUN_TEST(file_test_write2());
    RUN_TEST(file_test_append());
    //RUN_TEST(file_test_reopen());
}
