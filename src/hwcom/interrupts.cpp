
#include <hwcom/interrupts.h>
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



InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
)
{

    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;

}

// Note 2024-10-31 i add something i frogot and then nanami-sama makes my project into the most erroneous shit. this file currently has literally 100 fucking errors lmaooooo

InterruptManager::InterruptManager(uint16_t HardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager)
: picLeadCommand(0x20), // Placed a semicolon here, but realized it's supposed to be a normal colon. Silly me! | .|
  picLeadData(0x21), // placed colons separately here. could tell before the guy in the tutorial because vs code (what i'm using to code the entire os) coloured the first one of these four different from the rest
  picFollowCommand(0xA0),
  picFollowData(0xA1) 
{
    this->taskManager = taskManager;
//    this->hardwareInterruptOffset = hardwareInterruptOffset; this might be of note someday, i don't think so.
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint16_t i = 0; i < 256; i++)
    {
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    // HandleException0x?? wouldn't compile.

/*
    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);
*/

    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x22, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x23, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x24, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x25, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x26, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x27, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x28, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x29, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

    picLeadCommand.Write(0x11);
    picFollowCommand.Write(0x11);

    picLeadData.Write(0x20);
    picFollowData.Write(0x28);

    picLeadData.Write(0x04);
    picFollowData.Write(0x02);

    picLeadData.Write(0x01);
    picFollowData.Write(0x01);

    picLeadData.Write(0x00);
    picFollowData.Write(0x00);

    interruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));

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

void InterruptManager::CheckIDTVector(uint8_t vector)
{
    idtR idtr;
    asm volatile ("sidt %0" : "=m"(idtr));

    uint8_t *idt = (uint8_t*) (uintptr_t) idtr.base;
    uint8_t *entry = idt + (vector * 8);

    uint16_t offset_low  = *(uint16_t*)(entry + 0);
    uint16_t selector    = *(uint16_t*)(entry + 2);
    uint8_t  typeattr    = *(uint8_t*)(entry + 5);
    uint16_t offset_high = *(uint16_t*)(entry + 6);

    uint32_t offset = ((uint32_t)offset_high << 16) | offset_low;

    if (offset == 0)
        printf("No IDT entry!");
    else
        printf("IDT entry OK!");
}

void InterruptManager::CheckIDTAttribAndSlctr(uint8_t vector)
{
    idtR idtr;
    asm volatile ("sidt %0" : "=m"(idtr));

    uint8_t *idt = (uint8_t*) (uintptr_t) idtr.base;
    uint8_t *entry = idt + (vector * 8);
    uint16_t selector    = *(uint16_t*)(entry + 2);
    uint8_t  typeattr    = *(uint8_t*)(entry + 5);

    if (typeattr == 0x8E) printf(" The IDT attribute is OK. "); else printf(" The IDT attribute failed. ");
    if (selector == 0x08) printf("The IDT selector is OK. "); else printf("The IDT selector failed. ");
}
/*
void InterruptManager::DebugStage5()
{
    uint8_t lea = inb(0x21);
    uint8_t fol = inb(0xA1);

    if ((lea & 1) == 0) printf("");
}
*/
void InterruptManager::Activate()
{
    if(ActiveInterruptManager != 0)
        ActiveInterruptManager->Deactivate();
    ActiveInterruptManager = this;
    asm("sti");

    printf(" asm-sti ");
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
    else if(interruptNumber != 0x20) // assuming it's not a timer interrupt?
    {
        char* foo = "\nAn unhandled interrupt has been made. Please write an entry for: 0x00";
        char* hex = "0123456789ABCDEF";
        foo[70] = hex[(interruptNumber >> 4) & 0x0F];
        foo[71] = hex[interruptNumber & 0x0F];
        printf(foo);
    }

    if(0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        // let's try long-term to see if this layout is *better*. feels counter-intuitive.
        if(0x28 <= interruptNumber)
            picFollowCommand.Write(0x20);
        picLeadCommand.Write(0x20);
    }

    return esp;

}
