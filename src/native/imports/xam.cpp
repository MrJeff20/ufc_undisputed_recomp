#include "ppc_context.h"
#include "../runtime_control.h"

namespace
{
constexpr uint32_t kSystemVersion = 0x200A0000;
constexpr uint32_t kLanguageEnglish = 1;
}

PPC_FUNC(__imp__XamGetSystemVersion)
{
    ctx.r3.u64 = kSystemVersion;
}

PPC_FUNC(__imp__XGetLanguage)
{
    ctx.r3.u64 = kLanguageEnglish;
}
PPC_FUNC(__imp__XamLoaderTerminateTitle)
{
    throw TitleTermination{};
}