
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
                osos::common::uint16_t limit_lo;
                osos::common::uint16_t base_lo;
                osos::common::uint8_t base_hi;
                osos::common::uint8_t type;
                osos::common::uint8_t flags_limit_hi;
                osos::common::uint8_t base_vhi;
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
