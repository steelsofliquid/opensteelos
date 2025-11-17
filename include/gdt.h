
#ifndef __GDT_H
#define __GDT_H

#include <common/types.h>

namespace osos
{
    class GlobalDescriptorTable
    {

        public:
            class SegmentDescriptor
            {
            private:
                osos::common::uint16_t limitLow;
                osos::common::uint16_t baseLow;
                osos::common::uint8_t baseHigh;
                osos::common::uint8_t type;
                osos::common::uint8_t flagsLimitHigh;
                osos::common::uint8_t baseVirtualHigh;
            public:
                SegmentDescriptor(osos::common::uint32_t base, osos::common::uint32_t limit, osos::common::uint8_t type);
                osos::common::uint32_t Base();
                osos::common::uint32_t Limit();

            } __attribute__((packed));


        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        osos::common::uint16_t CodeSegmentSelector();
        osos::common::uint16_t DataSegmentSelector();
        


    };
}


#endif
