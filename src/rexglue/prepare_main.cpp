#include "ufc_image.h"

#include <filesystem>
#include <exception>
#include <csignal>
#include <iostream>
#include <string_view>
#include <utility>
#include <rex/kernel/init.h>
#include <rex/memory.h>
#include <rex/runtime.h>
#include <rex/system/xthread.h>

#if defined(_WIN32)
#include <Windows.h>

namespace {
void DumpNativeStack(const char* reason) {
  void* frames[32]{};
  const auto count = CaptureStackBackTrace(0, 32, frames, nullptr);
  std::fprintf(stderr, "Native fatal: %s (thread=%lu)\n", reason, GetCurrentThreadId());
  for (USHORT i = 0; i < count; ++i) {
    MEMORY_BASIC_INFORMATION info{};
    if (!VirtualQuery(frames[i], &info, sizeof(info))) continue;
    char module[MAX_PATH]{};
    GetModuleFileNameA(static_cast<HMODULE>(info.AllocationBase), module, MAX_PATH);
    const auto offset = reinterpret_cast<uintptr_t>(frames[i]) -
                        reinterpret_cast<uintptr_t>(info.AllocationBase);
    std::fprintf(stderr, "  %s+0x%llX\n", module,
                 static_cast<unsigned long long>(offset));
  }
  std::fflush(stderr);
}

[[noreturn]] void ReportTerminate() {
  DumpNativeStack("std::terminate");
  TerminateProcess(GetCurrentProcess(), 0xE0000001u);
  __builtin_unreachable();
}

void ReportPureCall() {
  DumpNativeStack("pure virtual call");
  TerminateProcess(GetCurrentProcess(), 0xE0000002u);
}

void ReportAbort(int) {
  DumpNativeStack("SIGABRT");
  TerminateProcess(GetCurrentProcess(), 0xE0000004u);
}

void ReportInvalidParameter(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int,
                            uintptr_t) {
  DumpNativeStack("invalid CRT parameter");
  TerminateProcess(GetCurrentProcess(), 0xE0000003u);
}

LONG CALLBACK ReportException(EXCEPTION_POINTERS* exception) {
  const auto* record = exception->ExceptionRecord;
  const auto image_base = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
  const auto address = reinterpret_cast<uintptr_t>(record->ExceptionAddress);
  std::fprintf(stderr, "Native exception 0x%08lX at RVA 0x%llX\n",
               record->ExceptionCode,
               static_cast<unsigned long long>(address - image_base));
  return EXCEPTION_CONTINUE_SEARCH;
}
}  // namespace
#endif

int main(int argc, char** argv) {
#if defined(_WIN32)
  AddVectoredExceptionHandler(1, ReportException);
  std::set_terminate(ReportTerminate);
  _set_purecall_handler(ReportPureCall);
  _set_invalid_parameter_handler(ReportInvalidParameter);
  std::signal(SIGABRT, ReportAbort);
#endif
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  const std::filesystem::path game_root = argc > 1 ? argv[1] : "extracted";
  const bool run_entry = argc > 2 && std::string_view(argv[2]) == "--run-entry";
  const auto xex_path = game_root / "default.xex";
  if (!std::filesystem::is_regular_file(xex_path)) {
    std::cerr << "default.xex not found under " << game_root << '\n';
    return 1;
  }

  rex::Runtime runtime(game_root);
  rex::RuntimeConfig config{
      .kernel_init = rex::kernel::InitializeKernel,
      .tool_mode = true,
  };
  if (XFAILED(runtime.Setup(UFC3PPCImageConfig, std::move(config)))) {
    std::cerr << "ReXGlue runtime setup failed\n";
    return 2;
  }
  if (XFAILED(runtime.LoadXexImage("game:\\default.xex"))) {
    std::cerr << "default.xex failed to load\n";
    return 3;
  }

  constexpr uint32_t kJumpTableAddress = 0x827A0980;
  const auto* jump_table = runtime.memory()->TranslateVirtual(kJumpTableAddress);
  std::cerr << "Jump table at 0x827A0980:\n";
  for (uint32_t index = 0; index < 15; ++index) {
    const auto value = rex::memory::load_and_swap<uint32_t>(jump_table + index * 4);
    std::fprintf(stderr, "  [%02u] 0x%08X\n", index, value);
  }

  auto main_thread = runtime.PrepareModuleLaunch();
  if (!main_thread) {
    std::cerr << "Entry thread preparation failed\n";
    return 4;
  }

  std::cout << "UFC3 entry thread prepared at 0x" << std::hex << kUFC3EntryPoint
            << " (not resumed)\n";
if (!run_entry) {
    return 0;
  }

  std::cout << "Resuming UFC3 entry thread\n";
  if (XFAILED(main_thread->Resume())) {
    std::cerr << "Entry thread resume failed\n";
    return 5;
  }
  main_thread->Wait(0, 0, 0, nullptr);
  std::cout << "UFC3 entry thread exited\n";
  return 0;
}