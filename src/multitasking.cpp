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

void printf(char* str, ...);

// Class Task

extern volatile uint32_t tickcount;

Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;

    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;

    cpustate -> error = 0;
    */
    // cpustate -> esp = 0;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ds = 0;
    cpustate -> eflags = 0x202;

    IsAsleep = false;
    WakeTick = 0;
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

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    //printf("\nScheduling a task...");
    if(numTasks <= 0)
        return cpustate;

    if(currentTask >= 0)
        tasks[currentTask] -> cpustate = cpustate;

    for (int i = 0; i < numTasks; i++)
    {
        if(++currentTask >= numTasks)
            currentTask %= numTasks;
        
        if(!tasks[currentTask] -> IsAsleep)
            return tasks[currentTask] -> cpustate;
    }

    //printf(" done!");
    
    return cpustate;
}


void TaskManager::sleep(uint32_t interval)
{
    const uint32_t freqPIT = 100;
    uint32_t intervalInTicks = interval / 10;

    Task* TaskToSleep = tasks[currentTask];
    TaskToSleep->IsAsleep = true;
    TaskToSleep->WakeTick = tickcount + intervalInTicks;
}

void TaskManager::WakeTask(uint32_t ticks)
{
    for (int i = 0; i < numTasks; i++)
    {
        if (tasks[i] -> IsAsleep && ticks >= tasks[i] -> WakeTick)
            tasks[i] -> IsAsleep = false;
    }
}

namespace osos{
    TaskManager taskManager; // we sent this piece from the kernel off to the gulag, otherwise known as where the class this thing is defined in. on that thought why don't i do this for shit like the libraries?
}