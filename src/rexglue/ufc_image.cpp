#include "ufc_image.h"

#include "ppc_config.h"
#include <rex/ppc/func.h>

const rex::PPCImageInfo UFC3PPCImageConfig = {
    .code_base = static_cast<rex::u32>(PPC_CODE_BASE),
    .code_size = static_cast<rex::u32>(PPC_CODE_SIZE),
    .image_base = static_cast<rex::u32>(PPC_IMAGE_BASE),
    .image_size = static_cast<rex::u32>(PPC_IMAGE_SIZE),
    .func_mappings = PPCFuncMappings,
};

static_assert(kUFC3EntryPoint == PPC_ENTRY_POINT);