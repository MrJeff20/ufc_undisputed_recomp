#include "xenonrecomp_abi.h"

#include <cstdio>

REX_EXTERN(__imp__sub_823C96D8);
REX_EXTERN(__imp__sub_823C92B8);
REX_EXTERN(__imp__sub_823C7C80);
REX_EXTERN(__imp__sub_82F01B28);
REX_EXTERN(__imp__sub_823C9588);
REX_EXTERN(__imp__sub_823C94A8);
REX_EXTERN(__imp__sub_823C9498);
REX_EXTERN(__imp__sub_8236B020);
REX_EXTERN(__imp__sub_82F01858);
REX_EXTERN(__imp__sub_827A7198);
REX_EXTERN(__imp__sub_828D5820);
REX_EXTERN(__imp__sub_827A6258);
REX_EXTERN(__imp__sub_823C37D0);
REX_EXTERN(__imp__sub_823C3870);
REX_EXTERN(__imp__sub_8236AF58);
REX_EXTERN(__imp__sub_823C3018);
REX_EXTERN(__imp__sub_823C3F70);
REX_EXTERN(__imp__sub_823C3BA8);
REX_EXTERN(__imp__sub_823C3010);
REX_EXTERN(__imp__sub_823C2BD0);
REX_EXTERN(__imp__sub_823C9090);
REX_EXTERN(__imp__sub_823CA8A0);
REX_EXTERN(__imp__sub_823C8DC0);

namespace {
void Trace(const char* phase, const char* function) {
  std::fprintf(stderr, "[startup] %s %s\n", phase, function);
  std::fflush(stderr);
}
}  // namespace

#define UFC_TRACE_WRAPPER(name)                  \
  extern "C" REX_FUNC(name) {                   \
    Trace("enter", #name);                       \
    __imp__##name(ctx, base);                    \
    Trace("leave", #name);                       \
  }

UFC_TRACE_WRAPPER(sub_823C96D8)
UFC_TRACE_WRAPPER(sub_823C92B8)
UFC_TRACE_WRAPPER(sub_823C7C80)
UFC_TRACE_WRAPPER(sub_82F01B28)
UFC_TRACE_WRAPPER(sub_823C9588)
UFC_TRACE_WRAPPER(sub_823C94A8)
UFC_TRACE_WRAPPER(sub_823C9498)
UFC_TRACE_WRAPPER(sub_8236B020)
UFC_TRACE_WRAPPER(sub_82F01858)
UFC_TRACE_WRAPPER(sub_827A7198)
UFC_TRACE_WRAPPER(sub_828D5820)
UFC_TRACE_WRAPPER(sub_827A6258)
UFC_TRACE_WRAPPER(sub_823C37D0)
UFC_TRACE_WRAPPER(sub_823C3870)
UFC_TRACE_WRAPPER(sub_8236AF58)
UFC_TRACE_WRAPPER(sub_823C3018)
UFC_TRACE_WRAPPER(sub_823C3F70)
UFC_TRACE_WRAPPER(sub_823C3BA8)
UFC_TRACE_WRAPPER(sub_823C3010)
UFC_TRACE_WRAPPER(sub_823C2BD0)
UFC_TRACE_WRAPPER(sub_823C9090)
UFC_TRACE_WRAPPER(sub_823CA8A0)
UFC_TRACE_WRAPPER(sub_823C8DC0)