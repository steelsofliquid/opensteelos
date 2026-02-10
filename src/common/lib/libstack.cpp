#include <common/lib/libstack.h>

using namespace osos;





StringStack::StringStack(size_t capacity)
{
    const char* stack[capacity];
    int32_t item;
}

StringStack::~StringStack()
{
}


void StringStack::push(char* str)
{
}

void StringStack::pop()
{
}





IntStack::IntStack(size_t capacity)
{
    int* stack[capacity];
    int32_t item;
}

IntStack::~IntStack()
{
}


void IntStack::push(int value)
{
}

void IntStack::pop()
{
}





HexStack::HexStack(size_t capacity)
{
    uint16_t* stack[capacity];
    int32_t item;
}

HexStack::~HexStack()
{
}


void HexStack::push(uint16_t value)
{
}

void HexStack::pop()
{
}





UnsignedIntStack::UnsignedIntStack(size_t capacity)
{
    uint32_t* stack[capacity];
    int32_t item;
}

UnsignedIntStack::~UnsignedIntStack()
{
}


void UnsignedIntStack::push(uint32_t value)
{
}

void UnsignedIntStack::pop()
{
}