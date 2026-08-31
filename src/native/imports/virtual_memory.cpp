#include "ppc_context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <atomic>

namespace
{
constexpr uint32_t kPageSize = 0x1000;
constexpr uint32_t kDynamicBase = 0x20000000;
constexpr uint32_t kDynamicLimit = 0x50000000;
constexpr uint32_t kStatusNoMemory = 0xC0000017;
std::atomic<uint32_t> gNextVirtualAddress{kDynamicBase};

constexpr uint32_t AlignUp(uint32_t value)
{
    return (value + kPageSize - 1) & ~(kPageSize - 1);
}
}

PPC_FUNC(__imp__NtAllocateVirtualMemory)
{
    const uint32_t addressPointer = ctx.r3.u32;
    const uint32_t sizePointer = ctx.r4.u32;
    uint32_t requestedAddress = PPC_LOAD_U32(addressPointer);
    uint32_t requestedSize = PPC_LOAD_U32(sizePointer);
    if (requestedSize == 0)
    {
        ctx.r3.u64 = kStatusNoMemory;
        return;
    }

    const uint32_t pageOffset = requestedAddress & (kPageSize - 1);
    const uint32_t committedSize = AlignUp(requestedSize + pageOffset);
    uint32_t guestAddress = requestedAddress & ~(kPageSize - 1);
    if (requestedAddress == 0)
    {
        guestAddress = gNextVirtualAddress.fetch_add(committedSize);
        if (guestAddress >= kDynamicLimit ||
            committedSize > kDynamicLimit - guestAddress)
        {
            ctx.r3.u64 = kStatusNoMemory;
            return;
        }
    }

    if (VirtualAlloc(base + guestAddress, committedSize, MEM_COMMIT,
                     PAGE_READWRITE) == nullptr)
    {
        ctx.r3.u64 = kStatusNoMemory;
        return;
    }

    PPC_STORE_U32(addressPointer, guestAddress);
    PPC_STORE_U32(sizePointer, committedSize);
    ctx.r3.u64 = 0;
}