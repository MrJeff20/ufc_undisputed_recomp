#include "memory.h"

#include "ppc_config.h"
#include "ppc_context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <fstream>

namespace
{
constexpr size_t kAddressSpaceSize = 1ull << 32;
constexpr size_t kFunctionTableSize = PPC_CODE_SIZE * 2;
}

NativeMemory::NativeMemory()
{
    base_ = static_cast<uint8_t*>(
        VirtualAlloc(nullptr, kAddressSpaceSize, MEM_RESERVE, PAGE_NOACCESS));
    if (base_ != nullptr &&
        (VirtualAlloc(base_ + PPC_IMAGE_BASE, PPC_IMAGE_SIZE, MEM_COMMIT,
                      PAGE_READWRITE) == nullptr ||
         VirtualAlloc(base_ + PPC_IMAGE_BASE + PPC_IMAGE_SIZE,
                      kFunctionTableSize, MEM_COMMIT, PAGE_READWRITE) == nullptr))
    {
        VirtualFree(base_, 0, MEM_RELEASE);
        base_ = nullptr;
    }
}

NativeMemory::~NativeMemory()
{
    if (base_ != nullptr)
    {
        VirtualFree(base_, 0, MEM_RELEASE);
    }
}

uint8_t* NativeMemory::image() const
{
    return base_ + PPC_IMAGE_BASE;
}
bool NativeMemory::loadImage(const std::filesystem::path& path)
{
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    if (!stream || stream.tellg() != static_cast<std::streamoff>(PPC_IMAGE_SIZE))
    {
        return false;
    }
    stream.seekg(0);
    stream.read(reinterpret_cast<char*>(image()), PPC_IMAGE_SIZE);
    return stream.good();
}

std::size_t NativeMemory::initializeFunctionTable(const PPCFuncMapping* mappings)
{
    std::size_t count = 0;
    for (; mappings[count].host != nullptr; ++count)
    {
        const auto guest = static_cast<uint32_t>(mappings[count].guest);
        if (guest < PPC_CODE_BASE || guest >= PPC_CODE_BASE + PPC_CODE_SIZE)
        {
            continue;
        }
        auto slot = reinterpret_cast<PPCFunc**>(
            base_ + PPC_IMAGE_BASE + PPC_IMAGE_SIZE +
            (static_cast<uint64_t>(guest) - PPC_CODE_BASE) * 2);
        *slot = mappings[count].host;
    }
    return count;
}