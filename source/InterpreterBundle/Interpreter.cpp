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

#include <ORM/ORM.h>
#include <ErrorBundle/ErrorLog.h>
#include <InterpreterBundle/Interpreter.h>

Interpreter::Interpreter(uint64_t id, std::string fname) : Object(id)
{
    //TOOO: do something here
}

void
Interpreter::addThread(Method *m)
{
    auto id = Interpreter::next_id++;

    Thread *t = Thread::create(id, m);

    Interpreter::threads[id] = std::thread([&]() {
        t->run();

        if (errorLogIsEmpty())
        {
            Interpreter::removeThread(id);
        }
        else
        {
            Interpreter::stop();
        }

        ORM::destroy(t);
    });

    Interpreter::threads[id].join();
}

void
Interpreter::run()
{
    while (!Interpreter::threads.empty())
    {}
}

void
Interpreter::removeThread(uint32_t id)
{
    Interpreter::threads.erase(id);
}

void
Interpreter::stop()
{
    for (auto &t : Interpreter::threads) {
        //TOOO: do something here
        t.second.detach();
    }
}
