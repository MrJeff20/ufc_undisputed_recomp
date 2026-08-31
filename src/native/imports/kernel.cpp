#include "ppc_context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

PPC_FUNC(__imp__KeQueryPerformanceFrequency)
{
    LARGE_INTEGER frequency{};
    QueryPerformanceFrequency(&frequency);
    ctx.r3.u64 = static_cast<uint64_t>(frequency.QuadPart);
}

PPC_FUNC(__imp__KeQuerySystemTime)
{
    FILETIME time{};
    GetSystemTimeAsFileTime(&time);
    const uint64_t ticks =
        (static_cast<uint64_t>(time.dwHighDateTime) << 32) | time.dwLowDateTime;
    PPC_STORE_U64(ctx.r3.u32, ticks);
}
PPC_FUNC(__imp__KeTlsAlloc)
{
    ctx.r3.u64 = TlsAlloc();
}

PPC_FUNC(__imp__KeTlsSetValue)
{
    const auto value = reinterpret_cast<void*>(static_cast<uintptr_t>(ctx.r4.u64));
    ctx.r3.u64 = TlsSetValue(ctx.r3.u32, value) != FALSE;
}

PPC_FUNC(__imp__KeTlsGetValue)
{
    ctx.r3.u64 = reinterpret_cast<uintptr_t>(TlsGetValue(ctx.r3.u32));
}

PPC_FUNC(__imp__KeTlsFree)
{
    ctx.r3.u64 = TlsFree(ctx.r3.u32) != FALSE;
}
PPC_FUNC(__imp__KeGetCurrentProcessType)
{
    ctx.r3.u64 = 1;
}