/*
    This is the OpenSteel/OS Task Manager. You do not open it if you have
    an issue with a program. For context, the Microsoft Windows NT Task Manager
    was developed by Dave Plummer in the 90's, as a program to manage open processes.
    An honest name would for it in my books would be "Windows Task Assistant", as it
    would assist the real "task manager" in the forme of a GUI providing open tasks
    and killing them.

    This code is for a formal task manager. It schedules, creates and will eventually
    be able to kill tasks, among other things. When there's eventually a component
    similar to Windows Task Manager, that component will be called the Task Assistant.
*/

#include <multitasking.h>
#include <drivers/pit.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;

extern volatile uint32_t tickCount;

void printf(char* str, ...);





// Class Task

Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpuState = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpuState -> eax = 0;
    cpuState -> ebx = 0;
    cpuState -> ecx = 0;
    cpuState -> edx = 0;

    cpuState -> esi = 0;
    cpuState -> edi = 0;
    cpuState -> ebp = 0;

    /*
    cpuState -> gs = 0;
    cpuState -> fs = 0;
    cpuState -> es = 0;
    cpuState -> ds = 0;

    cpuState -> error = 0;
    */
    // cpuState -> esp = 0;
    cpuState -> eip = (uint32_t)entrypoint;
    cpuState -> cs = gdt->CodeSegmentSelector();
    // cpuState -> ds = 0;
    cpuState -> eflags = 0x202;

    isAsleep = false;
    wakeTick = 0;
}

Task::~Task()
{
}




// Class TaskManager

TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false;
    tasks[numTasks++] = task;
    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpuState)
{
    if(numTasks <= 0)
        return cpuState;

    if(currentTask >= 0)
        tasks[currentTask] -> cpuState = cpuState;

    for (int i = 0; i < numTasks; i++)
    {
        if(++currentTask >= numTasks)
            currentTask %= numTasks;
        
        if(!tasks[currentTask] -> isAsleep)
            return tasks[currentTask] -> cpuState;
    }
    
    return cpuState;
}


void TaskManager::sleep(uint32_t interval)
{
    const uint32_t freqPIT = 100;
    uint32_t intervalInTicks = interval / 10;

    Task* TaskToSleep = tasks[currentTask];
    TaskToSleep->isAsleep = true;
    TaskToSleep->wakeTick = tickCount + intervalInTicks;
}

void TaskManager::WakeTask(uint32_t ticks)
{
    for (int i = 0; i < numTasks; i++)
    {
        if (tasks[i] -> isAsleep && ticks >= tasks[i] -> wakeTick)
            tasks[i] -> isAsleep = false;
    }
}

namespace osos
{
    TaskManager taskManager; // we sent this piece from the kernel off to the gulag, otherwise known as where the class this thing is defined in. on that thought why don't i do this for shit like the libraries?
}
