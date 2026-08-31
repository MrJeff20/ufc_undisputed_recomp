#include "fallback.h"

#include "ppc_context.h"

#include <iostream>
#include <mutex>
#include <string>
#include <unordered_set>

namespace
{
constexpr uint64_t kNotImplemented = 0xC0000002;
std::mutex gFallbackMutex;
std::unordered_set<std::string> gReportedFallbacks;
}

void NativeImportFallback(PPCContext& context, const char* name)
{
    {
        std::lock_guard lock(gFallbackMutex);
        if (gReportedFallbacks.emplace(name).second)
        {
            std::cerr << "Native import fallback: " << name << '\n';
        }
    }
    context.r3.u64 = kNotImplemented;
}