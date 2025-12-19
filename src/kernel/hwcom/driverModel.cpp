#include <kernel/hwcom/driverModel.h>

using namespace osos;
using namespace osos::common;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

void printf(char* str, ...);





DriverModel::DriverModel()
{
    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;
}

DriverModel::~DriverModel()
{
}



InterruptHandler* DriverModel::InterruptHandlerForme()
{
    return nullptr;
}



void DriverModel::StartDriver()
{
}



void DriverModel::Initialise()
{
    if (driverAttributes.isInitialised == true) return; // If already initialised, we just return
    // TODO later: add helpers for setting up ports, allocating memory and whatnot
    // as of circuit 5 build 23x, a memory expedition still needs to be conducted.

    driverAttributes.isInitialised = true;
}

void DriverModel::Activate()
{
    if (driverAttributes.isInitialised != true) Initialise();
    if (driverAttributes.isActive      == true) return;

    if (driverAttributes.hasInterruptRequest == true)
    {
        // TODO later: incorporate the main interrupt management logic in here instead of drivers doing it themselves
    }

    StartDriver();

    driverAttributes.isActive = true;
}

void DriverModel::Deactivate()
{
    if (driverAttributes.isActive == false) return;
    // May need a helper to make this happen in an orderly fashion
    if (driverAttributes.hasInterruptRequest == true)
    {
        // TODO later: Same ordeal as DriverModel::Activate()
    }

    driverAttributes.isActive = false;
}

void DriverModel::Reset()
{
    // TODO: This is a very crude implementation. It doesn't account for memory management,
    //       or anything to be honest.

    Deactivate();
    Initialise();
    Activate();
}



const char* DriverModel::GetDriverName()
{
    return driverAttributes.name;
}

const char* DriverModel::GetDriverType()
{
    return driverAttributes.type;
}



bool DriverModel::IsDriverInitialised()
{
    return driverAttributes.isInitialised;
}

bool DriverModel::IsDriverActive()
{
    return driverAttributes.isActive;
}