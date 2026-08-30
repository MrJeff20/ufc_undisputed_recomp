#pragma once 

#include <cassert>
#include <cstdint>

#ifdef _MSC_VER
#include <cstdlib>
#define __builtin_bswap16 _byteswap_ushort
#define __builtin_bswap32 _byteswap_ulong
#define __builtin_bswap64 _byteswap_uint64
#endif

template<typename T>
inline T ByteSwap(T value)
{
    if constexpr (sizeof(T) == 1)
        return value;
    else if constexpr (sizeof(T) == 2)
        return static_cast<T>(__builtin_bswap16(static_cast<uint16_t>(value)));
    else if constexpr (sizeof(T) == 4)
        return static_cast<T>(__builtin_bswap32(static_cast<uint32_t>(value)));
    else if constexpr (sizeof(T) == 8)
        return static_cast<T>(__builtin_bswap64(static_cast<uint64_t>(value)));

    assert(false && "Unexpected byte size.");
    return value;
}

template<typename T>
inline void ByteSwapInplace(T& value)
{
    value = ByteSwap(value);
}
