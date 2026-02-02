#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/idt.h>
#include <kernel/hwcom/pic.h>

using namespace osos;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

InterruptHandler::InterruptHandler(InterruptManager* interruptManager, uint8_t interruptNumber)
{
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;

    if (this->interruptManager) // safeguard
        this->interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler()
{
    if(interruptManager->handlers[interruptNumber] == this)
        interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
    return esp;
}

void InterruptHandler::attachToInterruptManager(InterruptManager* interruptManager)
{
    // carried over from InterruptHandler
    this->interruptManager = interruptManager;

    if (this->interruptManager)
        this->interruptManager->handlers[interruptNumber] = this;
}


ProgrammableInterruptController programmableInterruptController;
InterruptDescriptorTable interruptDescriptorsBench;               // couldn't call it interruptDescriptorTable >_<

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

// Note 2024-10-31 i add something i frogot and then nanami-sama makes my project into the most erroneous shit. this file currently has literally 100 fucking errors lmaooooo

InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager)
{
    this->taskManager = taskManager;
//    this->hardwareInterruptOffset = hardwareInterruptOffset; this might be of note someday, i don't think so.
    uint16_t codeSegment = gdt->CodeSegmentSelector();
    
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint16_t i = 0; i < 256; i++)
    {
        handlers[i] = 0;
        interruptDescriptorsBench.SetInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    interruptDescriptorsBench.SetExceptions(gdt);
    interruptDescriptorsBench.SetInterruptRequests(gdt);

    programmableInterruptController.Initialise();
    interruptDescriptorsBench.Initialise();

}

InterruptManager::~InterruptManager() // I'm leaving this in, the comment said "oops i forgot a semicolon". This became such a fixation I rarely do that now.
// *Builds nuclear reactor software* "oops i forgot a semicolon and it compiled anyway" *world devastation*
{
}


bool InterruptManager::handlerExists(uint8_t interruptNumber)
{
    return handlers[interruptNumber] != nullptr;
}

bool InterruptManager::interruptsEnabled()
{
    uint64_t flags;
    asm volatile
    (
        "pushf\n\t"
        "pop %0"
        : "=g" (flags)
    );

    return flags & (1 << 9);
}

void InterruptManager::Activate()
{
    if(ActiveInterruptManager != 0)
        ActiveInterruptManager->Deactivate();
    ActiveInterruptManager = this;
    asm("sti");
}

void InterruptManager::Deactivate()
{
    if(ActiveInterruptManager == this)
    {
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if(ActiveInterruptManager != 0)
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    // Originally, interrupts were viewed as a "worst-case" scenario in pre-Neisa OpenSteel/OS (i.e. NanamiOS).
    // This isn't REALLY true, interrupts are, in this case, being hardware comm, integral to the OS.
    return esp;

}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if(interruptNumber == 0x27 || interruptNumber == 0x2F) // is it spurious?
    {
        if (interruptNumber == 0x27) // IRQ 7
        {
            outb(0x20, 0x0B);
            uint8_t isrBit = inb(0x20);
            if (!(isrBit & (1 << 7))) return esp;
        }
        else // IRQ 15
        {
            outb(0xA0, 0x0B);
            uint8_t isrBit = inb(0xA0);
            if (!(isrBit & (1 << 7)))
            {
                outb(0x20, 0x20);
                return esp;
            }
        }
    }

    // else statement is not ideal here

    if(handlers[interruptNumber] != 0)
    {
        esp = handlers[interruptNumber]->HandleInterrupt(esp); // should call the appropriate function per driver.
    }
    else if (interruptNumber <= 0x1F)
    {
        panic(interruptNumber);
    }
    else if(interruptNumber != 0x20) // assuming it's not a timer interrupt?
    {
        printf("\nUnhandled interrupt. Please write an entry for: 0x%x", interruptNumber);
    }

    if(0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        uint8_t irq = interruptNumber - 0x20;
        programmableInterruptController.SendEOI(interruptNumber);
    }

    return esp;

}
