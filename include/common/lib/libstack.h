// Based off of my own stack functions as they were in Python
// from the late 2023 Python course
//
// The thing is, Python has some significant differences to
// the low-level C/C++ thing that is the OpenSteel/OS source code.
// As such, I don't know if I'll have to write multiple push functions
// or figure out a system that tells what type is in the thing.

#ifndef __OSOS__COMMON__LIB__LIBSTACK_H
#define __OSOS__COMMON__LIB__LIBSTACK_H

#include <common/types.h>
#include <kernel/mem/dmm.h>

// This library (libstack.h) defines a sort of pseudo-type. Stacks are memory data structures (for instance, an array or list stored in memory) and so putting its functions in a base unclassified library is less than ideal.
class StringStack // This library (libstack.h) defines a sort of pseudo-type. Stacks are memory data structures. This class is designed for string-type stacks in particular.
{
    public:
    StringStack(size_t capacity);
    ~StringStack();

    void push(const char* str);
    void pop();
};

#endif