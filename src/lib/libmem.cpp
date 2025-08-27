
// The first library to be developed for OpenSteel/OS. It contains memory functions.
// If you're looking for malloc and free, those are already defined in dmm.cpp and its header.

#include <lib/libmem.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;

MemoryLibrary::MemoryLibrary()
{
}

MemoryLibrary::~MemoryLibrary()
{
}

void* MemoryLibrary::memcpy(void* destination, const void* source, size_t number)
{
}

void* MemoryLibrary::memmove(void* destination, const void* source, size_t number)
{
}

void* MemoryLibrary::memset(void* destination, int32_t value, size_t number)
{
}

uint32_t MemoryLibrary::memcmp(const void* size1, const void* size2, size_t number)
{
}