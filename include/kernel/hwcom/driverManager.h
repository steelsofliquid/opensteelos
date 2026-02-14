#ifndef __OSOS__KERNEL__HWCOM__DRIVERMANAGER_H
#define __OSOS__KERNEL__HWCOM__DRIVERMANAGER_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/interrupts.h>

namespace osos
{
    namespace kernel
    {
        namespace hwcom
        {
            class DriverManager
            {
                private:
                DriverModel* drivers[255];
                int32_t driverCount;

                public:
                static DriverManager *activeDriverManager;
                
                DriverManager();
                ~DriverManager();

                void RegisterInterruptDrivers(osos::kernel::hwcom::InterruptManager* interruptMan);

                void AddDriver(DriverModel* driver);
                void RemoveDriver(DriverModel* driver);
                void ActivateAll(osos::kernel::hwcom::InterruptManager* interruptMan);
                void DeactivateAll();

                DriverModel* GetDriverByName(const char* name);
                DriverModel* GetDriverByType(const char* type);
            };
        }
    }
}

#endif