#include "ppc_context.h"

#include <memory>
#include <mutex>
#include <unordered_map>

namespace
{
std::mutex gCriticalSectionsMutex;
std::unordered_map<uint32_t, std::shared_ptr<std::recursive_mutex>> gCriticalSections;

std::shared_ptr<std::recursive_mutex> GetCriticalSection(uint32_t guestAddress)
{
    std::lock_guard lock(gCriticalSectionsMutex);
    auto& section = gCriticalSections[guestAddress];
    if (!section)
    {
        section = std::make_shared<std::recursive_mutex>();
    }
    return section;
}
}

PPC_FUNC(__imp__RtlInitializeCriticalSection)
{
    GetCriticalSection(ctx.r3.u32);
    ctx.r3.u64 = 0;
}

PPC_FUNC(__imp__RtlInitializeCriticalSectionAndSpinCount)
{
    GetCriticalSection(ctx.r3.u32);
    ctx.r3.u64 = 0;
}

PPC_FUNC(__imp__RtlEnterCriticalSection)
{
    GetCriticalSection(ctx.r3.u32)->lock();
    ctx.r3.u64 = 0;
}

PPC_FUNC(__imp__RtlTryEnterCriticalSection)
{
    ctx.r3.u64 = GetCriticalSection(ctx.r3.u32)->try_lock();
}

PPC_FUNC(__imp__RtlLeaveCriticalSection)
{
    GetCriticalSection(ctx.r3.u32)->unlock();
    ctx.r3.u64 = 0;
}