#include "ppc_context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <atomic>
#include <mutex>
#include <unordered_map>

namespace
{
constexpr uint32_t kPoolBase = 0x10000000;
constexpr uint32_t kPoolLimit = 0x70000000;
constexpr uint32_t kPageSize = 0x1000;
std::atomic<uint32_t> gNextPoolAddress{kPoolBase};
std::mutex gPoolMutex;
std::unordered_map<uintptr_t, size_t> gPoolAllocations;

constexpr uint32_t AlignToPage(uint32_t size)
{
    return (size + kPageSize - 1) & ~(kPageSize - 1);
}
}

PPC_FUNC(__imp__ExAllocatePoolTypeWithTag)
{
    const uint32_t committedSize = AlignToPage(ctx.r4.u32 == 0 ? 1 : ctx.r4.u32);
    const uint32_t guestAddress = gNextPoolAddress.fetch_add(committedSize);
    if (guestAddress > kPoolLimit || committedSize > kPoolLimit - guestAddress)
    {
        ctx.r3.u64 = 0;
        return;
    }

    void* allocation = VirtualAlloc(base + guestAddress, committedSize, MEM_COMMIT,
                                    PAGE_READWRITE);
    if (allocation == nullptr)
    {
        ctx.r3.u64 = 0;
        return;
    }

    {
        std::lock_guard lock(gPoolMutex);
        gPoolAllocations[reinterpret_cast<uintptr_t>(allocation)] = committedSize;
    }
    ctx.r3.u64 = guestAddress;
}

PPC_FUNC(__imp__ExFreePool)
{
    const auto allocation = reinterpret_cast<uintptr_t>(base + ctx.r3.u32);
    size_t committedSize = 0;
    {
        std::lock_guard lock(gPoolMutex);
        const auto it = gPoolAllocations.find(allocation);
        if (it == gPoolAllocations.end())
        {
            return;
        }
        committedSize = it->second;
        gPoolAllocations.erase(it);
    }
    VirtualFree(reinterpret_cast<void*>(allocation), committedSize, MEM_DECOMMIT);
}