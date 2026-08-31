#include "ppc_context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <string_view>

PPC_FUNC(__imp__DbgPrint)
{
    const auto text = reinterpret_cast<const char*>(base + ctx.r3.u32);
    MEMORY_BASIC_INFORMATION info{};
    if (VirtualQuery(text, &info, sizeof(info)) == 0 || info.State != MEM_COMMIT)
    {
        ctx.r3.u64 = 0xC0000005;
        return;
    }

    const auto regionEnd = static_cast<const char*>(info.BaseAddress) + info.RegionSize;
    size_t length = 0;
    while (text + length < regionEnd && length < 4096 && text[length] != '\0')
    {
        ++length;
    }

    std::cerr << "[UFC3] " << std::string_view(text, length);
    if (length == 0 || text[length - 1] != '\n')
    {
        std::cerr << '\n';
    }
    ctx.r3.u64 = static_cast<uint32_t>(length);
}