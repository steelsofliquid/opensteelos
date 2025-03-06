
#ifndef __OSOS__HWCOM__PORT_H
#define __OSOS__HWCOM__PORT_H

#include <common/types.h>

namespace osos
{
    namespace hwcom
    {
        class Port
        {
        protected:
        osos::common::uint16_t portnumber;
            Port(osos::common::uint16_t portnumber); // Must be virtual when kernel gets memory management
            ~Port();
        };

        class Port8Bit : public Port
        {
        public:
            Port8Bit(osos::common::uint16_t portnumber);
            ~Port8Bit();
            virtual osos::common::uint8_t Read();
            virtual void Write(osos::common::uint8_t data);
        };

        class Port8BitSlow : public Port8Bit
        {
        public:
            Port8BitSlow(osos::common::uint16_t portnumber);
            ~Port8BitSlow();
            virtual void Write(osos::common::uint8_t data);
        };

        class Port16Bit : public Port
        {
        public:
            Port16Bit(osos::common::uint16_t portnumber);
            ~Port16Bit();
            virtual void Write(osos::common::uint16_t data);
            virtual osos::common::uint16_t Read();
        };

        class Port32Bit : public Port
        {
        public:
            Port32Bit(osos::common::uint16_t portnumber);
            ~Port32Bit();
            virtual void Write(osos::common::uint32_t data);
            virtual osos::common::uint32_t Read();
        };
    }
}

#endif
