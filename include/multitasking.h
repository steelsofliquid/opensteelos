
#ifndef __MULTITASKING_H
#define __MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace osos
{
    struct CPUState
    {
        osos::common::uint32_t eax;
        osos::common::uint32_t ebx;
        osos::common::uint32_t ecx;
        osos::common::uint32_t edx;

        osos::common::uint32_t esi;
        osos::common::uint32_t edi;
        osos::common::uint32_t ebp;

        osos::common::uint32_t error;

        osos::common::uint32_t eip;
        osos::common::uint32_t cs;
        osos::common::uint32_t eflags;
        osos::common::uint32_t esp;
        osos::common::uint32_t ss;

    } __attribute__((packed));

    class Task
    {
    friend class TaskManager;
    private:
        osos::common::uint8_t stack[4096]; // 4 KiB
        CPUState* cpustate;
    public:
        Task(GlobalDescriptorTable *gdt, void entrypoint());
        ~Task();
    };

    class TaskManager
    {
    private:
        Task* tasks[256];
        int numTasks;
        int currentTask;
    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task* task);
        CPUState* Schedule(CPUState* cpustate);
    };
    
}

#endif
