#include "types.hpp"
#include "boot.hpp"
#include "terminal.hpp"
#include "memory/memory.hpp"
#include "interrupts/interrupts.hpp"

BasicTerminal* pTerminal = nullptr;

void Panic(const char* message)
{
    pTerminal->set_background(0x003276E3);
    pTerminal->set_foreground(0x00FFFFFF);
    
    pTerminal->clear();
    pTerminal->print("KERNEL PANIC.\n");
    pTerminal->print("Error message: ", message, "\n");
    pTerminal->print("System shutdown.\n");

    while(true) {
        __asm("cli");
        __asm("hlt");
    }
}

[[gnu::ms_abi]]
extern "C" void KernelMain(Boot::data* bootData)
{
    Boot::systemFont& systemFont = *bootData->pSystemFont;
    Boot::screenData& screenData = *bootData->pScreenData;
    Boot::memoryData& memoryData = bootData->memoryData;

    BasicTerminal terminal(screenData, systemFont);
    terminal.clear();
    terminal.print("Hello, world!\n");
    pTerminal = &terminal;

    Memory::runtime memoryRuntime(*bootData, terminal);

    Interrupt::specialRegister idtr;
    Interrupt::initialize(memoryRuntime, idtr);

//    int* asd = (int*)0x8000000000;
//    *asd = 99;

    terminal.clear();
    terminal.print("Hello once more!\n");

    while (true) {}
}
