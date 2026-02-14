#include <kernel/hwcom/driverManager.h>

using namespace osos;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;





DriverManager* DriverManager::activeDriverManager = 0;

// These allow for the driver manager to cycle through drivers
int driverCycle      = 0;
int driverCycleSaved = 0;

DriverManager::DriverManager()
{
    driverCount = 0;
    drivers[255] = nullptr; 

    activeDriverManager = this;
}

DriverManager::~DriverManager()
{
    if (activeDriverManager == this) activeDriverManager = 0;
}



void DriverManager::RegisterInterruptDrivers(InterruptManager* interruptMan)
{
}



void DriverManager::AddDriver(DriverModel* driver)
{
    drivers[driverCycle] = driver;

    driverCount++;
    driverCycle++;
    driverCycleSaved++;
}

void DriverManager::RemoveDriver(DriverModel* driver)
{
    while (drivers[driverCycle] != driver) driverCycle--; // cycle until right driver found
    drivers[driverCycle]->Deactivate();                   // make sure it is safe to remove driver

    drivers[driverCycle] = nullptr;

    driverCycle = driverCycleSaved;                       // "my job here is done"
    driverCount--;
}

void DriverManager::ActivateAll(InterruptManager* interruptMan)
{
    for (int i = 0; i < driverCount; i++)
    {
        if (drivers[i]->driverAttributes.isInitialised != true) drivers[i]->Initialise();

        if (drivers[i]->driverAttributes.hasInterruptRequest)
        {
            if (auto* interruptHand = drivers[i]->InterruptHandlerForme())
            {
                interruptHand->attachToInterruptManager(interruptMan);
            }
        }

        if (drivers[i]->driverAttributes.isActive == true) printf("\nHuh? driver already activated\n"); else drivers[i]->Activate();
    }
}

void DriverManager::DeactivateAll()
{
    for (int i = 0; i < driverCount; i++)
    {
        if (drivers[i]->driverAttributes.isActive == true) drivers[i]->Deactivate();
    }
}



DriverModel* DriverManager::GetDriverByName(const char* name)
{
    for (int i = 0; i < driverCount; i++)
    {
        if (drivers[i]->driverAttributes.name == name) return drivers[i];
    }
}

DriverModel* DriverManager::GetDriverByType(const char* type)
{
    for (int i = 0; i < driverCount; i++)
    {
        if (drivers[i]->driverAttributes.type == type) return drivers[i];
    }
}