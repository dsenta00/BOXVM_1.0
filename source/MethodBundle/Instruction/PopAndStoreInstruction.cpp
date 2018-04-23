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
#include <ORM/Relationship.h>
#include <ORM/MasterRelationships.h>
#include <ErrorBundle/ErrorLog.h>
#include <VariableBundle/Primitive/Primitive.h>
#include <MethodBundle/Method.h>
#include <MethodBundle/Instruction/PopAndStoreInstruction.h>
#include <codecvt>
#include <locale>

/**
 * @inherit
 */
PopAndStoreInstruction::PopAndStoreInstruction(std::vector<std::wstring> &arg)
    : Instruction(OP_CODE_POP_AND_STORE, arg)
{
}

/**
 * @inherit
 */
Instruction *
PopAndStoreInstruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto *m = this->getMethod();
    auto *data2 = m->pop();

    Value *data = (Value *) m->getVar(this->arg[0]);
    *data = *data2;

    return (Instruction *) this->getMaster()->get("next_instruction")->front();
}

/**
 * @inherit
 */
PopAndStoreInstruction *
PopAndStoreInstruction::create(std::wstring name)
{
    std::vector<std::wstring> arg;
    arg.emplace_back(name);

    return dynamic_cast<PopAndStoreInstruction *>(ORM::create(
        new PopAndStoreInstruction(arg)
    ));
}

/**
 * @inherit
 */
bool
PopAndStoreInstruction::validate()
{
    if (this->validated)
    {
        return true;
    }

    auto *m = this->getMethod();

    if (!m)
    {
        return false;
    }

    auto *data2 = (Primitive *) m->pop();

    if (!data2)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return false;
    }

    Primitive *data = (Primitive *) m->getVar(this->arg[0]);

    if (!data)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return false;
    }

    this->validated = true;
    return true;
}
