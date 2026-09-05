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
REX_EXTERN(__imp__sub_82F27128);
REX_EXTERN(__imp__sub_82F282C8);
REX_EXTERN(__imp__sub_83022458);
REX_EXTERN(__imp__sub_830224B8);
REX_EXTERN(__imp__sub_82FF67D8);
REX_EXTERN(__imp__sub_82F9B4D0);
REX_EXTERN(__imp__sub_82F5FB68);
REX_EXTERN(__imp__sub_82F5FC08);
REX_EXTERN(__imp__sub_82F972F8);
REX_EXTERN(__imp__sub_82F97FA8);
REX_EXTERN(__imp__sub_82F97CF0);
REX_EXTERN(__imp__sub_82F9B618);
REX_EXTERN(__imp__sub_82F9B678);
REX_EXTERN(__imp__sub_82F9B7A8);
REX_EXTERN(__imp__sub_82F9B810);
REX_EXTERN(__imp__sub_82F9B878);

namespace {
bool IsRepeatTrackedFunction(const char* function) {
  return std::strcmp(function, "sub_823C3010") == 0 ||
         std::strcmp(function, "sub_823C9090") == 0 ||
         std::strcmp(function, "sub_823CA8A0") == 0 ||
         std::strcmp(function, "sub_828D5820") == 0 ||
         std::strcmp(function, "sub_82F27128") == 0 ||
         std::strcmp(function, "sub_82F282C8") == 0 ||
         std::strcmp(function, "sub_83022458") == 0 ||
         std::strcmp(function, "sub_830224B8") == 0 ||
         std::strcmp(function, "sub_82FF67D8") == 0 ||
         std::strcmp(function, "sub_82F9B4D0") == 0 ||
         std::strcmp(function, "sub_82F5FB68") == 0 ||
         std::strcmp(function, "sub_82F5FC08") == 0 ||
         std::strcmp(function, "sub_82F972F8") == 0 ||
         std::strcmp(function, "sub_82F97FA8") == 0 ||
         std::strcmp(function, "sub_82F97CF0") == 0 ||
         std::strcmp(function, "sub_82F9B618") == 0 ||
         std::strcmp(function, "sub_82F9B678") == 0 ||
         std::strcmp(function, "sub_82F9B7A8") == 0 ||
         std::strcmp(function, "sub_82F9B810") == 0 ||
         std::strcmp(function, "sub_82F9B878") == 0;
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

bool Trace(const char* phase, const char* function, uint64_t* traced_count = nullptr) {
  uint32_t thread_id = rex::thread::current_thread_id();
  uint64_t call_count = 0;
  if (std::strcmp(phase, "enter") == 0 &&
      !ShouldTraceRepeatedCall(thread_id, function, &call_count)) {
    return false;
  }

  if (traced_count) {
    *traced_count = call_count;
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

void TraceProducerState(const char* function, uint64_t count, PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32;
  if (!count || !r3) {
    return;
  }

  if (std::strcmp(function, "sub_82F27128") == 0) {
    std::fprintf(stderr,
                 "[startup-state] tid=%08X %s count=%llu obj=%08X flag40=%02X flag64=%02X "
                 "read=%08X write=%08X signal=%08X\n",
                 rex::thread::current_thread_id(), function,
                 static_cast<unsigned long long>(count), r3, PPC_LOAD_U8(r3 + 40),
                 PPC_LOAD_U8(r3 + 64), PPC_LOAD_U32(r3 + 140), PPC_LOAD_U32(r3 + 144),
                 PPC_LOAD_U32(r3 + 164));
  } else if (std::strcmp(function, "sub_82F282C8") == 0) {
    std::fprintf(stderr,
                 "[startup-state] tid=%08X %s count=%llu obj=%08X frame=%08X signal=%08X "
                 "queue_read=%08X queue_write=%08X\n",
                 rex::thread::current_thread_id(), function,
                 static_cast<unsigned long long>(count), r3, PPC_LOAD_U32(r3 + 136),
                 PPC_LOAD_U32(r3 + 164), PPC_LOAD_U32(r3 + 140), PPC_LOAD_U32(r3 + 144));
  } else if (std::strcmp(function, "sub_83022458") == 0) {
    std::fprintf(stderr,
                 "[startup-state] tid=%08X %s count=%llu obj=%08X event40=%08X field16=%08X "
                 "field20=%08X field24=%08X field60=%08X\n",
                 rex::thread::current_thread_id(), function,
                 static_cast<unsigned long long>(count), r3, PPC_LOAD_U32(r3 + 40),
                 PPC_LOAD_U32(r3 + 16), PPC_LOAD_U32(r3 + 20), PPC_LOAD_U32(r3 + 24),
                 PPC_LOAD_U32(r3 + 60));
  } else if (std::strcmp(function, "sub_830224B8") == 0) {
    std::fprintf(stderr,
                 "[startup-state] tid=%08X %s count=%llu obj=%08X event40=%08X wait44=%08X "
                 "wait48=%08X frames56=%016llX\n",
                 rex::thread::current_thread_id(), function,
                 static_cast<unsigned long long>(count), r3, PPC_LOAD_U32(r3 + 40),
                 PPC_LOAD_U32(r3 + 44), PPC_LOAD_U32(r3 + 48),
                 static_cast<unsigned long long>(PPC_LOAD_U64(r3 + 56)));
  }
  std::fflush(stderr);
}

#define UFC_TRACE_STATE_WRAPPER(name)                     \
  extern "C" REX_FUNC(name) {                             \
    uint64_t traced_count = 0;                             \
    bool trace_leave = Trace("enter", #name, &traced_count); \
    TraceProducerState(#name, traced_count, ctx, base);    \
    __imp__##name(ctx, base);                              \
    if (trace_leave) {                                     \
      Trace("leave", #name);                              \
    }                                                      \
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
UFC_TRACE_STATE_WRAPPER(sub_82F27128)
UFC_TRACE_STATE_WRAPPER(sub_82F282C8)
UFC_TRACE_STATE_WRAPPER(sub_83022458)
UFC_TRACE_STATE_WRAPPER(sub_830224B8)

void TraceQueueCounterFunction(const char* function, uint64_t count, uint32_t owner, uint8_t* base,
                               bool before_call) {
  if (!count || owner < 0x40000000 || owner >= 0xC0000000) {
    return;
  }

  std::fprintf(stderr,
               "[startup-queue-counter] tid=%08X %s %s count=%llu owner=%08X vtbl=%08X "
               "field4=%08X field48=%08X field56=%08X field68=%08X\n",
               rex::thread::current_thread_id(), before_call ? "before" : "after", function,
               static_cast<unsigned long long>(count), owner, PPC_LOAD_U32(owner + 0),
               PPC_LOAD_U32(owner + 4), PPC_LOAD_U32(owner + 48), PPC_LOAD_U32(owner + 56),
               PPC_LOAD_U32(owner + 68));
  std::fflush(stderr);
}

#define UFC_TRACE_QUEUE_COUNTER_WRAPPER(name)              \
  extern "C" REX_FUNC(name) {                             \
    uint64_t traced_count = 0;                             \
    bool trace_leave = Trace("enter", #name, &traced_count); \
    const uint32_t owner = ctx.r3.u32;                     \
    TraceQueueCounterFunction(#name, traced_count, owner, base, true); \
    __imp__##name(ctx, base);                              \
    TraceQueueCounterFunction(#name, traced_count, owner, base, false); \
    if (trace_leave) {                                     \
      Trace("leave", #name);                              \
    }                                                      \
  }
UFC_TRACE_QUEUE_COUNTER_WRAPPER(sub_82F9B618)
UFC_TRACE_QUEUE_COUNTER_WRAPPER(sub_82F9B678)
UFC_TRACE_QUEUE_COUNTER_WRAPPER(sub_82F9B7A8)
UFC_TRACE_QUEUE_COUNTER_WRAPPER(sub_82F9B810)
UFC_TRACE_QUEUE_COUNTER_WRAPPER(sub_82F9B4D0)
UFC_TRACE_QUEUE_COUNTER_WRAPPER(sub_82F9B878)
void TraceSchedulerSource(const char* function, uint64_t count, uint32_t lr, uint32_t owner,
                          uint32_t arg4, uint32_t ret, uint8_t* base, bool before_call) {
  if (!count || owner < 0x40000000 || owner >= 0xC0000000) {
    return;
  }

  std::fprintf(stderr,
               "[startup-scheduler-source] tid=%08X %s %s count=%llu lr=%08X owner=%08X "
               "vtbl=%08X flag64=%02X list80=%08X field136=%08X field140=%08X field144=%08X "
               "field164=%08X ret=%08X arg4=%08X\n",
               rex::thread::current_thread_id(), before_call ? "before" : "after", function,
               static_cast<unsigned long long>(count), lr, owner, PPC_LOAD_U32(owner + 0),
               PPC_LOAD_U8(owner + 64), PPC_LOAD_U32(owner + 80), PPC_LOAD_U32(owner + 136),
               PPC_LOAD_U32(owner + 140), PPC_LOAD_U32(owner + 144), PPC_LOAD_U32(owner + 164),
               ret, arg4);
  std::fflush(stderr);
}

#define UFC_TRACE_SCHEDULER_SOURCE_WRAPPER(name)           \
  extern "C" REX_FUNC(name) {                             \
    uint64_t traced_count = 0;                             \
    bool trace_leave = Trace("enter", #name, &traced_count); \
    const uint32_t owner = ctx.r3.u32;                     \
    const uint32_t arg4 = ctx.r4.u32;                      \
    const uint32_t lr = static_cast<uint32_t>(ctx.lr);     \
    TraceSchedulerSource(#name, traced_count, lr, owner, arg4, 0, base, true); \
    __imp__##name(ctx, base);                              \
    TraceSchedulerSource(#name, traced_count, lr, owner, arg4, ctx.r3.u32, base, false); \
    if (trace_leave) {                                     \
      Trace("leave", #name);                              \
    }                                                      \
  }
UFC_TRACE_SCHEDULER_SOURCE_WRAPPER(sub_82F972F8)
UFC_TRACE_SCHEDULER_SOURCE_WRAPPER(sub_82F97FA8)
UFC_TRACE_SCHEDULER_SOURCE_WRAPPER(sub_82F97CF0)

void TraceNotifyLoopBlock(const char* function, uint64_t count, uint32_t block, uint8_t* base,
                          bool before_call) {
  if (!count || block < 0x40000000 || block >= 0xC0000000) {
    return;
  }

  std::fprintf(stderr,
               "[startup-notify-loop] tid=%08X %s %s count=%llu block=%08X "
               "h0=%08X h1=%08X h2=%08X stop12=%02X\n",
               rex::thread::current_thread_id(), before_call ? "before" : "after", function,
               static_cast<unsigned long long>(count), block, PPC_LOAD_U32(block + 0),
               PPC_LOAD_U32(block + 4), PPC_LOAD_U32(block + 8), PPC_LOAD_U8(block + 12));
  std::fflush(stderr);
}

#define UFC_TRACE_NOTIFY_LOOP_WRAPPER(name)                \
  extern "C" REX_FUNC(name) {                             \
    uint64_t traced_count = 0;                             \
    bool trace_leave = Trace("enter", #name, &traced_count); \
    const uint32_t block = ctx.r3.u32;                     \
    TraceNotifyLoopBlock(#name, traced_count, block, base, true); \
    __imp__##name(ctx, base);                              \
    TraceNotifyLoopBlock(#name, traced_count, block, base, false); \
    if (trace_leave) {                                     \
      Trace("leave", #name);                              \
    }                                                      \
  }
UFC_TRACE_NOTIFY_LOOP_WRAPPER(sub_82F5FB68)
UFC_TRACE_NOTIFY_LOOP_WRAPPER(sub_82F5FC08)

extern "C" REX_FUNC(sub_82FF67D8) {
  uint64_t traced_count = 0;
  bool trace_leave = Trace("enter", "sub_82FF67D8", &traced_count);
  const uint32_t count = ctx.r3.u32;
  const uint32_t handles = ctx.r4.u32;
  const uint32_t wait_all = ctx.r5.u32;
  const uint32_t timeout = ctx.r6.u32;
  const uint32_t alertable = ctx.r7.u32;
  const uint32_t call_owner = ctx.r30.u32;
  const uint32_t caller_lr = static_cast<uint32_t>(ctx.lr);
  const uint32_t raw_r30 = ctx.r30.u32;
  const uint32_t raw_r31 = ctx.r31.u32;
  if (traced_count) {
    std::fprintf(stderr,
                 "[startup-waitmulti-call] tid=%08X count=%u handles=%08X waitAll=%u "
                 "timeout=%08X alertable=%u callerLR=%08X r30=%08X r31=%08X h0=%08X h1=%08X h2=%08X h3=%08X\n",
                 rex::thread::current_thread_id(), count, handles, wait_all, timeout, alertable, caller_lr, raw_r30, raw_r31,
                 count > 0 ? PPC_LOAD_U32(handles + 0) : 0,
                 count > 1 ? PPC_LOAD_U32(handles + 4) : 0,
                 count > 2 ? PPC_LOAD_U32(handles + 8) : 0,
                 count > 3 ? PPC_LOAD_U32(handles + 12) : 0);
    std::fflush(stderr);
  }
  __imp__sub_82FF67D8(ctx, base);
  if (traced_count) {
    std::fprintf(stderr, "[startup-waitmulti-return] tid=%08X result=%08X\n",
                 rex::thread::current_thread_id(), ctx.r3.u32);
    const uint32_t owner = call_owner;
    if (owner >= 0x40000000 && owner < 0xC0000000) {
      std::fprintf(stderr,
                   "[startup-waitmulti-owner] tid=%08X result=%08X owner=%08X vtbl=%08X "
                   "field4=%08X field40=%08X field44=%08X field48=%08X field52=%08X "
                   "field56=%08X field68=%08X handles=%08X\n",
                   rex::thread::current_thread_id(), ctx.r3.u32, owner, PPC_LOAD_U32(owner + 0),
                   PPC_LOAD_U32(owner + 4), PPC_LOAD_U32(owner + 40), PPC_LOAD_U32(owner + 44),
                   PPC_LOAD_U32(owner + 48), PPC_LOAD_U32(owner + 52), PPC_LOAD_U32(owner + 56),
                   PPC_LOAD_U32(owner + 68), handles);
    }
    std::fflush(stderr);
  }
  if (trace_leave) {
    Trace("leave", "sub_82FF67D8");
  }
}

