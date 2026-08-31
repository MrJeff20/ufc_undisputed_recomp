#include "ufc_image.h"

#include <rex/ppc/func.h>
#include <rex/system/function_dispatcher.h>

#if defined(_WIN32)
#define UFC_REX_EXPORT __declspec(dllexport)
#else
#define UFC_REX_EXPORT __attribute__((visibility("default")))
#endif

extern "C" UFC_REX_EXPORT void ReXModule_Register(
    rex::runtime::IModuleRegistrar* registrar) {
  for (const PPCFuncMapping* mapping = PPCFuncMappings; mapping->guest != 0; ++mapping) {
    if (mapping->host != nullptr) {
      registrar->SetFunction(static_cast<uint32_t>(mapping->guest), mapping->host);
    }
  }
}

extern "C" UFC_REX_EXPORT const rex::PPCImageInfo* ReXModule_GetImageInfo() {
  return &UFC3PPCImageConfig;
}