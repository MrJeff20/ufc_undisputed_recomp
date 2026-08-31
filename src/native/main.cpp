#include "ppc_context.h"
#include "memory.h"

#include <iomanip>
#include <iostream>

PPC_EXTERN_FUNC(__imp__XamGetSystemVersion);
PPC_EXTERN_FUNC(__imp__XGetLanguage);
PPC_EXTERN_FUNC(__imp__KeQueryPerformanceFrequency);
PPC_EXTERN_FUNC(__imp__KeQuerySystemTime);
PPC_EXTERN_FUNC(__imp__KeTlsAlloc);
PPC_EXTERN_FUNC(__imp__KeTlsSetValue);
PPC_EXTERN_FUNC(__imp__KeTlsGetValue);
PPC_EXTERN_FUNC(__imp__KeTlsFree);

int main(int argc, char** argv)
{
    NativeMemory memory;
    if (!memory.valid())
    {
        std::cerr << "Failed to reserve native game address space.\n";
        return 1;
    }
    const auto imagePath = argc > 1 ? argv[1] : "ppc_output/ppc_image.bin";
    if (!memory.loadImage(imagePath))
    {
        std::cerr << "Failed to load game image: " << imagePath << '\n';
        return 2;
    }

    std::size_t linkedFunctions = 0;
    while (PPCFuncMappings[linkedFunctions].host != nullptr)
    {
        ++linkedFunctions;
    }
    const auto mappedFunctions = memory.initializeFunctionTable(PPCFuncMappings);

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

    std::cout << "UFC Undisputed 3 native bootstrap\n"
              << "Compiled translation units: " << UFC_RECOMP_SOURCE_COUNT << '\n'
              << "Linked PPC functions: " << linkedFunctions << '\n'
              << "Indirect function mappings: " << mappedFunctions << '\n'
              << "Game image: loaded (entry word 0x" << std::hex
              << entryWord << ")\n"
              << "System version import: 0x" << std::hex << systemVersion << '\n'
              << "Language import: " << std::dec << language << '\n'
              << "Performance frequency: " << performanceFrequency << '\n'
              << "System time ticks: " << systemTime << '\n'
              << "TLS round trip: "
              << (tlsValue == 0x1234 && tlsFreed ? "ok" : "failed") << '\n';
    return 0;
}