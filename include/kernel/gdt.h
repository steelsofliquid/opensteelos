
#ifndef __OSOS__KERNEL__GDT_H
#define __OSOS__KERNEL__GDT_H

#include <common/types.h>

namespace osos
{
    namespace kernel
    {
        class GlobalDescriptorTable
        {
            public:
            class SegmentDescriptor
            {
                private:
                uint16_t limitLow;
                uint16_t baseLow;
                uint8_t baseHigh;
                uint8_t type;
                uint8_t flagsLimitHigh;
                uint8_t baseVirtualHigh;

                public:
                SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
                uint32_t Base();
                uint32_t Limit();

            } __attribute__((packed));


            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

            public:
            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            uint16_t CodeSegmentSelector();
            uint16_t DataSegmentSelector();
    };
    }
}


#endif
