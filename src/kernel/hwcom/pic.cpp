#include <kernel/hwcom/pic.h>

using namespace osos;
using namespace osos::common;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

ProgrammableInterruptController::ProgrammableInterruptController()
: picLeadCommand   (0x20),
  picLeadData      (0x21),
  picFollowCommand (0xA0),
  picFollowData    (0xA1)
{
}

ProgrammableInterruptController::~ProgrammableInterruptController()
{
}



void ProgrammableInterruptController::Initialise()
{
    picLeadCommand.Write(0x11);
    picFollowCommand.Write(0x11);

    picLeadData.Write(0x20);
    picFollowData.Write(0x28);

    picLeadData.Write(0x04);
    picFollowData.Write(0x02);

    picLeadData.Write(0x01);
    picFollowData.Write(0x01);

    // unmask pic
    picLeadData.Write(0x00);
    picFollowData.Write(0x00);
}

void ProgrammableInterruptController::Disable()
{
    picLeadData.Write(0xFF);
    picFollowData.Write(0xFF);
}



void ProgrammableInterruptController::MaskIRQ(uint8_t interruptReq)
{
    uint8_t value;
    if (interruptReq < 8)
    {
        value = picLeadData.Read() | (1 << interruptReq);
        picLeadData.Write(value);
    }
    else
    {
        interruptReq -= 8;
        value = picFollowData.Read() | (1 << interruptReq);
        picFollowData.Write(value);
    }
}

void ProgrammableInterruptController::UnmaskIRQ(uint8_t interruptReq)
{
    uint8_t value;
    if (interruptReq < 8)
    {
        value = picLeadData.Read() | ~(1 << interruptReq);
        picLeadData.Write(value);
    }
    else
    {
        interruptReq -= 8;
        value = picFollowData.Read() | ~(1 << interruptReq);
        picFollowData.Write(value);
    }
}



void ProgrammableInterruptController::SendEOI(osos::common::uint8_t interruptRequest)
{
    if(8 <= interruptRequest)
        picFollowCommand.Write(0x20);
    picLeadCommand.Write(0x20);
}