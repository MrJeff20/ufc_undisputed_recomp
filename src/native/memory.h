#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>

struct PPCFuncMapping;

class NativeMemory
{
public:
    NativeMemory();
    ~NativeMemory();

    NativeMemory(const NativeMemory&) = delete;
    NativeMemory& operator=(const NativeMemory&) = delete;

    [[nodiscard]] bool valid() const { return base_ != nullptr; }
    [[nodiscard]] uint8_t* base() const { return base_; }
    [[nodiscard]] uint8_t* image() const;
    bool loadImage(const std::filesystem::path& path);
    std::size_t initializeFunctionTable(const PPCFuncMapping* mappings);

private:
    uint8_t* base_{};
};