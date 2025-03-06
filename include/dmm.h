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
            osos::common::size_t size;
        };

        class MemoryManager
        {

        protected:
            MemoryChunk* first;
            
        public:
            static MemoryManager *activeMemoryManager;

            MemoryManager(osos::common::size_t start, osos::common::size_t size);
            ~MemoryManager();

            void* malloc(osos::common::size_t size);
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

#endif
