// Memory Manager

#ifndef __DMM_H
#define __DMM_H

#include <common/types.h>

namespace osos
{
    struct MemoryChunk
    {
        MemoryChunk *next;
        MemoryChunk *prev;

        bool allocated;
        size_t size;
    };

    class MemoryManager
    {
        protected:
        MemoryChunk* first;
            
        public:
        static MemoryManager *activeMemoryManager;

        MemoryManager(size_t start, size_t size);
        ~MemoryManager();

        void* malloc(size_t size);
        void free(void* ptr);
    };
}

void* operator new(unsigned size);
void* operator new[](unsigned size);

//Called the placement new
void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);

void operator delete(void* ptr, unsigned size);
void operator delete[](void* ptr, unsigned size);

#endif
