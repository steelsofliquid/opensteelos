
#ifndef __MULTITASKING_H
#define __MULTITASKING_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <common/sysHelpers.h>
#include <kernel/gdt.h>

namespace osos
{
    namespace kernel
    {
        struct CPUState
        {
            uint32_t eax;
            uint32_t ebx;
            uint32_t ecx;
            uint32_t edx;

            uint32_t esi;
            uint32_t edi;
            uint32_t ebp;

            uint32_t error;

            uint32_t eip;
            uint32_t cs;
            uint32_t eflags;
            uint32_t esp;
            uint32_t ss;

        } __attribute__((packed));

        class Task
        {
            friend class TaskManager;

            private:
            uint8_t stack[4096]; // 4 KiB
            CPUState* cpuState;
            TaskEntry entry;
            void* context;

            public:
            bool isAsleep;
            ProcessStatuses taskStatus;
            uint32_t wakeTick;
            Task(GlobalDescriptorTable *gdt, TaskEntry entrypoint, void* contx);
            ~Task();
        };

        class TaskManager
        {
            private:
            Task* tasks[256];
            int numTasks;
            int currentTask;
            
            public:
            static TaskManager *activeTaskManager;

            TaskManager();
            ~TaskManager();

            bool AddTask(Task* task);
            Task* GetCurrentTask();

            CPUState* Schedule(CPUState* cpuState);
            void Yield();

            void WakeTask(uint32_t ticks);
            void RunCurrentTask();
            void EndCurrentTask();
        };

        void sleep(uint32_t interval);
        void TaskTrampoline();
    }
}

#endif
