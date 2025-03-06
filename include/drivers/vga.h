// This builds off of the mouse driver's base first xx lines as a framework.

#ifndef __OSOS__DRIVERS__VGA_H // Using the keyboard driver as a template
#define __OSOS__DRIVERS__VGA_H

    #include <common/types.h>
    #include <drivers/driver.h>
    #include <hwcom/port.h>
    // No need for interrupts.h here... I think.

    // Kyahahahaha!!!
    // Ah-lalalalalalalala!!

    // It's gonna be eleven-port madness!!

    namespace osos
    {
        namespace drivers{
            
            class VideoGraphicsArray
            {
            protected:
            // Kyaaa!! So many ports!! >.^
            // How da hail do you portray >_< and ^.^ in the same ASCII emoticon?
                osos::hwcom::Port8Bit MiscPort; // MP
                osos::hwcom::Port8Bit CRTCIndexPort; // CTRCIP
                osos::hwcom::Port8Bit CRTCDataPort; // CTRCDP
                osos::hwcom::Port8Bit SequencerIndexPort; // SIP
                osos::hwcom::Port8Bit SequencerDataPort; // SDP
                osos::hwcom::Port8Bit GCIndexPort; // GCIP (Graphics Controller Index Port)
                osos::hwcom::Port8Bit GCDataPort; // GCDP (Graphics Controller Data Port)
                osos::hwcom::Port8Bit ACIndexPort; // ACIP (Attribute Controller Index Port)
                osos::hwcom::Port8Bit ACReadPort; // ACReaP (Attribute Controller Read Port)
                osos::hwcom::Port8Bit ACWritePort; // ACWP (Attribute Controller Write Port)
                osos::hwcom::Port8Bit ACResetPort; // ACResP (Attribute Controller Reset Port)

                void WriteRegisters(osos::common::uint8_t* registers);
                osos::common::uint8_t* GetFrameBufferSegment();

                virtual osos::common::uint8_t GetColorIndex(osos::common::uint8_t r, osos::common::uint8_t g, osos::common::uint8_t b);
            
            public:
                VideoGraphicsArray();
                ~VideoGraphicsArray();

                virtual bool SupportsMode(osos::common::uint32_t width, osos::common::uint32_t height, osos::common::uint32_t colordepth);
                virtual bool SetMode(osos::common::uint32_t width, osos::common::uint32_t height, osos::common::uint32_t colordepth);
                virtual void PutPixel(osos::common::uint32_t x, osos::common::uint32_t y, osos::common::uint8_t r, osos::common::uint8_t g, osos::common::uint8_t b);
                virtual void PutPixel(osos::common::uint32_t x, osos::common::uint32_t y, osos::common::uint8_t colorIndex);

                virtual void FillRectangle(osos::common::uint32_t x, osos::common::uint32_t y, osos::common::uint32_t w, osos::common::uint32_t h,
                    osos::common::uint8_t r, osos::common::uint8_t g, osos::common::uint8_t b);

            };
        }
    }


#endif
