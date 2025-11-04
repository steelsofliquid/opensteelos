
#include <drivers/amd_am79c973.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;

PCNetFastIII::PCNetFastIII(PCIDevDesc dev, InterruptManager* interrupts)
{
}

PCNetFastIII::~PCNetFastIII()
{
}

void Activate()
{
}

int Reset()
{
}

uint32_t HandleInterrupt(uint32_t esp);