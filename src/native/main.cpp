#include "ppc_context.h"
#include "memory.h"
#include "runtime_control.h"

#include <iomanip>
#include <iostream>
#include <string_view>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace
{
uintptr_t gMemoryBase{};

LONG WINAPI ReportNativeCrash(EXCEPTION_POINTERS* exception)
{
    const auto* record = exception->ExceptionRecord;
    std::cerr << "Native crash: code 0x" << std::hex << record->ExceptionCode
              << ", host address " << record->ExceptionAddress;
    const auto moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    const auto instructionAddress =
        reinterpret_cast<uintptr_t>(record->ExceptionAddress);
    if (instructionAddress >= moduleBase)
    {
        std::cerr << " (RVA 0x" << instructionAddress - moduleBase << ')';
    }
    if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION &&
        record->NumberParameters >= 2)
    {
        const char* operation = record->ExceptionInformation[0] == 0
            ? "read"
            : record->ExceptionInformation[0] == 1 ? "write" : "execute";
        const auto faultAddress = record->ExceptionInformation[1];
        std::cerr << ", " << operation << " address 0x" << faultAddress;
        if (gMemoryBase != 0 && faultAddress >= gMemoryBase &&
            faultAddress - gMemoryBase < (1ull << 32))
        {
            std::cerr << " (guest 0x" << faultAddress - gMemoryBase << ')';
        }
    }
    std::cerr << '\n';
    return EXCEPTION_EXECUTE_HANDLER;
}
}

PPC_EXTERN_FUNC(__imp__XamGetSystemVersion);
PPC_EXTERN_FUNC(__imp__XGetLanguage);
PPC_EXTERN_FUNC(__imp__KeQueryPerformanceFrequency);
PPC_EXTERN_FUNC(__imp__KeQuerySystemTime);
PPC_EXTERN_FUNC(__imp__KeTlsAlloc);
PPC_EXTERN_FUNC(__imp__KeTlsSetValue);
PPC_EXTERN_FUNC(__imp__KeTlsGetValue);
PPC_EXTERN_FUNC(__imp__KeTlsFree);
PPC_EXTERN_FUNC(_xstart);

int main(int argc, char** argv)
{
    SetUnhandledExceptionFilter(ReportNativeCrash);
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    NativeMemory memory;
    if (!memory.valid())
    {
        std::cerr << "Failed to reserve native game address space.\n";
        return 1;
    }
    gMemoryBase = reinterpret_cast<uintptr_t>(memory.base());
    const bool runEntry = argc > 1 && std::string_view(argv[1]) == "--run-entry";
    const auto imagePath = !runEntry && argc > 1
        ? argv[1]
        : "ppc_output/ppc_image.bin";
    if (!memory.loadImage(imagePath))
    {
        std::cerr << "Failed to load game image: " << imagePath << '\n';
        return 2;
    }    if (!memory.initializeDataImports())
    {
        std::cerr << "Failed to initialize data imports.\n";
        return 3;
    }

    std::size_t linkedFunctions = 0;
    while (PPCFuncMappings[linkedFunctions].host != nullptr)
    {
        ++linkedFunctions;
    }
    const auto mappedFunctions = memory.initializeFunctionTable(PPCFuncMappings);
    PPCFunc* entryFunction = PPC_LOOKUP_FUNC(memory.base(), PPC_ENTRY_POINT);
    if (entryFunction != _xstart)
    {
        std::cerr << "Entry point mapping does not match _xstart.\n";
        return 3;
    }

    const auto entryWord = *reinterpret_cast<const uint32_t*>(
        memory.base() + PPC_ENTRY_POINT);

    PPCContext context{};
    __imp__XamGetSystemVersion(context, nullptr);
    const auto systemVersion = context.r3.u32;
    __imp__XGetLanguage(context, nullptr);
    const auto language = context.r3.u32;
    __imp__KeQueryPerformanceFrequency(context, memory.base());
    const auto performanceFrequency = context.r3.u64;
    context.r3.u64 = PPC_IMAGE_BASE + PPC_IMAGE_SIZE - sizeof(uint64_t);
    __imp__KeQuerySystemTime(context, memory.base());
    const auto systemTime = __builtin_bswap64(*reinterpret_cast<uint64_t*>(
        memory.base() + context.r3.u32));

    __imp__KeTlsAlloc(context, memory.base());
    const auto tlsIndex = context.r3.u32;
    context.r3.u64 = tlsIndex;
    context.r4.u64 = 0x1234;
    __imp__KeTlsSetValue(context, memory.base());
    context.r3.u64 = tlsIndex;
    __imp__KeTlsGetValue(context, memory.base());
    const auto tlsValue = context.r3.u64;
    context.r3.u64 = tlsIndex;
    __imp__KeTlsFree(context, memory.base());
    const bool tlsFreed = context.r3.u64 != 0;
    context.r1.u64 = memory.stackTop();

    std::cout << "UFC Undisputed 3 native bootstrap\n"
              << "Compiled translation units: " << UFC_RECOMP_SOURCE_COUNT << '\n'
              << "Linked PPC functions: " << linkedFunctions << '\n'
              << "Indirect function mappings: " << mappedFunctions << '\n'
              << "Data imports: initialized\n"
              << "Game image: loaded (entry word 0x" << std::hex
              << entryWord << ")\n"
              << "System version import: 0x" << std::hex << systemVersion << '\n'
              << "Language import: " << std::dec << language << '\n'
              << "Performance frequency: " << performanceFrequency << '\n'
              << "System time ticks: " << systemTime << '\n'
              << "TLS round trip: "
              << (tlsValue == 0x1234 && tlsFreed ? "ok" : "failed") << '\n'
              << "Initial stack pointer: 0x" << std::hex << context.r1.u32
              << '\n'
              << "Native entry point: ready\n";
    if (runEntry)
    {
        std::cout << "Starting native entry point...\n";
        try
        {
            entryFunction(context, memory.base());
            std::cout << "Native entry point returned.\n";
        }
        catch (const TitleTermination& error)
        {
            std::cout << error.what() << '\n';
        }
    }
    return 0;
}