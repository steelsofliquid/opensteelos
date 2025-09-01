#include <lib/libstr.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;



StringLibrary::StringLibrary()
{
}

StringLibrary::~StringLibrary()
{
}


size_t StringLibrary::strlen(const uint8_t* string)
{
}

int32_t StringLibrary::strcmp(const uint8_t* string1, const uint8_t* string2)
{
}

int32_t StringLibrary::strncmp(const uint8_t* string1, const uint8_t* string2, size_t n)
{
}


uint8_t* StringLibrary::strcpy(uint8_t* destination, const uint8_t* source)
{
}

uint8_t* StringLibrary::strncpy(uint8_t* destination, const uint8_t* source, size_t n)
{
}

uint8_t* StringLibrary::strcat(uint8_t* destination, const uint8_t* source)
{
}


uint8_t* StringLibrary::strchr(const uint8_t* string, uint32_t character)
{
}

uint8_t* StringLibrary::strrchr(const uint8_t* string, uint32_t character)
{
}

uint8_t* StringLibrary::strstr(const uint8_t* haystack, const uint8_t* needle)
{
}