#include <hwcom/interrupts.h>
#include <hwcom/pic.h>
#include <hwcom/idt.h>

using namespace osos;
using namespace osos::common;
using namespace osos::hwcom;

void printf(char* str, ...);
void printfHex(uint8_t);

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


bool InterruptManager::handlerExists(osos::common::uint8_t interruptNumber)
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
    if(handlers[interruptNumber] != 0)
    {
        esp = handlers[interruptNumber]->HandleInterrupt(esp); // should call the appropriate function per driver.
    }
    else if (interruptNumber = 0x06 || 0x0D)
    {
        // this is a makeshift crash handler. I need to redo the panic() function.
        printf("\nMAKESHIFT PANIC: 0x0D 0X06 GENERAL PROTECTION FAULT\nOpenSteel/OS encountered an error and needs to shut down.");
        while(1);
    }
    else if(interruptNumber != 0x20) // assuming it's not a timer interrupt?
    {
        // this semi-legacy code needs to some reworking. the original if statement was deleted, since it's contents were
        // transferred to pit.cpp. should be as simple as an if statement, but i'm not going to test my luck right now.

        char* foo = "\nUnhandled interrupt. Please write an entry for: 0x00";
        char* hex = "0123456789ABCDEF";
        foo[51] = hex[(interruptNumber >> 4) & 0x0F];
        foo[52] = hex[interruptNumber & 0x0F];
        printf(foo);
    }

    if(0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        programmableInterruptController.SendEOI(interruptNumber);
    }

    return esp;

}
