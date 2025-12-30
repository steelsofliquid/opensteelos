#ifndef __OSOS__DRIVERS__VGA_H
#define __OSOS__DRIVERS__VGA_H

#include <common/types.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace drivers
    { 
        class VideoGraphicsArray
        {
            protected:
            osos::kernel::hwcom::Port8Bit MiscPort; // MP
            osos::kernel::hwcom::Port8Bit CRTCIndexPort; // CTRCIP
            osos::kernel::hwcom::Port8Bit CRTCDataPort; // CTRCDP
            osos::kernel::hwcom::Port8Bit SequencerIndexPort; // SIP
            osos::kernel::hwcom::Port8Bit SequencerDataPort; // SDP
            osos::kernel::hwcom::Port8Bit GCIndexPort; // GCIP (Graphics Controller Index Port)
            osos::kernel::hwcom::Port8Bit GCDataPort; // GCDP (Graphics Controller Data Port)
            osos::kernel::hwcom::Port8Bit ACIndexPort; // ACIP (Attribute Controller Index Port)
            osos::kernel::hwcom::Port8Bit ACReadPort; // ACReaP (Attribute Controller Read Port)
            osos::kernel::hwcom::Port8Bit ACWritePort; // ACWP (Attribute Controller Write Port)
            osos::kernel::hwcom::Port8Bit ACResetPort; // ACResP (Attribute Controller Reset Port)

            void WriteRegisters(uint8_t* registers);
            uint8_t* GetFrameBufferSegment();

            virtual uint8_t GetColorIndex(uint8_t r, uint8_t g, uint8_t b);
            
            public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            virtual bool SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth);
            virtual bool SetMode(uint32_t width, uint32_t height, uint32_t colordepth);
            virtual void PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
            virtual void PutPixel(uint32_t x, uint32_t y, uint8_t colorIndex);

            virtual void FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                uint8_t r, uint8_t g, uint8_t b);

        };
    }
}


#endif
