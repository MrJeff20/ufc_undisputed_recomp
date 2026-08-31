#pragma once

#include "ppc_config.h"

#define REX_CONFIG_H_INCLUDED
#define REX_IMAGE_BASE PPC_IMAGE_BASE
#define REX_IMAGE_SIZE PPC_IMAGE_SIZE
#define REX_CODE_BASE PPC_CODE_BASE
#define REX_CODE_SIZE PPC_CODE_SIZE

#include <rex/ppc.h>

// Legacy XenonRecomp emits helper calls without the ReXGlue namespace.
using namespace rex::ppc;

// Prevent generated sources from reintroducing XenonRecomp's legacy context.
#define PPC_CONTEXT_H_INCLUDED

#define PPC_FUNC REX_FUNC
#define PPC_FUNC_IMPL(name) extern "C" REX_FUNC(name)
#define PPC_EXTERN_FUNC REX_EXTERN
#define PPC_WEAK_FUNC(name) __attribute__((weak, noinline)) REX_FUNC(name)
#define PPC_FUNC_PROLOGUE() __builtin_assume(((size_t)base & 0x1F) == 0)

#define PPC_LOAD_U8(x) (*(volatile uint8_t*)(base + (uint32_t)(x)))
#define PPC_LOAD_U16(x) __builtin_bswap16(*(volatile uint16_t*)(base + (uint32_t)(x)))
#define PPC_LOAD_U32(x) __builtin_bswap32(*(volatile uint32_t*)(base + (uint32_t)(x)))
#define PPC_LOAD_U64(x) __builtin_bswap64(*(volatile uint64_t*)(base + (uint32_t)(x)))
#define PPC_STORE_U8(x, y) (*(volatile uint8_t*)(base + (uint32_t)(x)) = (y))
#define PPC_STORE_U16(x, y) (*(volatile uint16_t*)(base + (uint32_t)(x)) = __builtin_bswap16(y))
#define PPC_STORE_U32(x, y) (*(volatile uint32_t*)(base + (uint32_t)(x)) = __builtin_bswap32(y))
#define PPC_STORE_U64(x, y) (*(volatile uint64_t*)(base + (uint32_t)(x)) = __builtin_bswap64(y))

#define PPC_MM_LOAD_U8 PPC_LOAD_U8
#define PPC_MM_LOAD_U16 PPC_LOAD_U16
#define PPC_MM_LOAD_U32 PPC_LOAD_U32
#define PPC_MM_LOAD_U64 PPC_LOAD_U64
#define PPC_MM_STORE_U8 PPC_STORE_U8
#define PPC_MM_STORE_U16 PPC_STORE_U16
#define PPC_MM_STORE_U32 PPC_STORE_U32
#define PPC_MM_STORE_U64 PPC_STORE_U64

#define PPC_CALL_FUNC(x) x(ctx, base)
#define PPC_CALL_INDIRECT_FUNC(x) rex::runtime::ResolveIndirectFunction(x)(ctx, base)
#define PPC_MEMORY_SIZE 0x100000000ull

#define PPC_ROUND_NEAREST rex::ppc::kRoundNearest
#define PPC_ROUND_TOWARD_ZERO rex::ppc::kRoundTowardZero
#define PPC_ROUND_UP rex::ppc::kRoundUp
#define PPC_ROUND_DOWN rex::ppc::kRoundDown
#define PPC_ROUND_MASK rex::ppc::kRoundMask
