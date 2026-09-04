#include "xenonrecomp_abi.h"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <string>
#include <unordered_map>

#include <rex/thread.h>

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
bool IsRepeatTrackedFunction(const char* function) {
  return std::strcmp(function, "sub_823C3010") == 0 ||
         std::strcmp(function, "sub_823C9090") == 0 ||
         std::strcmp(function, "sub_823CA8A0") == 0 ||
         std::strcmp(function, "sub_828D5820") == 0;
}

bool ShouldTraceRepeatedCall(uint32_t thread_id, const char* function, uint64_t* call_count) {
  static std::mutex mutex;
  static std::unordered_map<std::string, uint64_t> counts;

  if (!IsRepeatTrackedFunction(function)) {
    *call_count = 0;
    return true;
  }

  std::lock_guard<std::mutex> lock(mutex);
  std::string key = std::to_string(thread_id) + ":" + function;
  uint64_t count = ++counts[key];
  *call_count = count;
  return count <= 10 || (count % 1000) == 0;
}

bool Trace(const char* phase, const char* function) {
  uint32_t thread_id = rex::thread::current_thread_id();
  uint64_t call_count = 0;
  if (std::strcmp(phase, "enter") == 0 &&
      !ShouldTraceRepeatedCall(thread_id, function, &call_count)) {
    return false;
  }

  if (call_count) {
    std::fprintf(stderr, "[startup] tid=%08X %s %s count=%llu\n", thread_id, phase, function,
                 static_cast<unsigned long long>(call_count));
  } else {
    std::fprintf(stderr, "[startup] tid=%08X %s %s\n", thread_id, phase, function);
  }
  std::fflush(stderr);
  return true;
}
}  // namespace

#define UFC_TRACE_WRAPPER(name)                  \
  extern "C" REX_FUNC(name) {                   \
    bool trace_leave = Trace("enter", #name);    \
    __imp__##name(ctx, base);                    \
    if (trace_leave) {                           \
      Trace("leave", #name);                    \
    }                                            \
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

