// The OpenSteel/OS RS232 (COM and Serial) driver.

#include <drivers/rs232.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;

RecommendedStandard232Driver::RecommendedStandard232Driver() :
    COM1(0x3F8)
{
}

RecommendedStandard232Driver::~RecommendedStandard232Driver()
{
}

int32_t RecommendedStandard232Driver::InitialiseSerial()
{
}


int32_t RecommendedStandard232Driver::serialReceived()
{
}

int8_t RecommendedStandard232Driver::ReadSerial()
{
}

int32_t RecommendedStandard232Driver::isTransitEmpty()
{
}

void RecommendedStandard232Driver::WriteToSerial(uint8_t material)
{
}



void RecommendedStandard232Driver::Activate()
{
}

uint32_t RecommendedStandard232Driver::HandleInterrupt(uint32_t esp)
{
}