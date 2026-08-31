cmake_minimum_required(VERSION 3.25)

project(UFCUndisputed3NativePort LANGUAGES CXX)

set(UFC_PPC_OUTPUT_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/ppc_output"
    CACHE PATH
    "Directory containing XenonRecomp generated C++ output")

set(UFC_SIMDE_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/XenonRecomp-ufc/thirdparty/simde"
    CACHE PATH
    "SIMDe include directory used by generated ppc_context.h")

if (MSVC AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR
        "The generated PPC code currently requires Clang/LLVM builtins. "
        "Configure with clang++ or clang-cl, for example via scripts/03_build_incremental.bat.")
endif()

if (NOT EXISTS "${UFC_PPC_OUTPUT_DIR}/ppc_recomp_shared.h")
    message(FATAL_ERROR
        "Generated PPC output was not found at ${UFC_PPC_OUTPUT_DIR}. "
        "Run XenonRecomp before configuring this host build.")
endif()

if (NOT EXISTS "${UFC_SIMDE_INCLUDE_DIR}/x86/avx.h")
    message(FATAL_ERROR
        "SIMDe headers were not found at ${UFC_SIMDE_INCLUDE_DIR}. "
        "Expected x86/avx.h under that directory.")
endif()

file(GLOB UFC_RECOMP_SOURCES CONFIGURE_DEPENDS
    "${UFC_PPC_OUTPUT_DIR}/ppc_recomp.*.cpp")
list(SORT UFC_RECOMP_SOURCES)

add_library(ufc_ppc_generated OBJECT
    "${UFC_PPC_OUTPUT_DIR}/ppc_func_mapping.cpp"
    ${UFC_RECOMP_SOURCES})

target_compile_features(ufc_ppc_generated PRIVATE cxx_std_20)

target_include_directories(ufc_ppc_generated PRIVATE
    "${UFC_PPC_OUTPUT_DIR}"
    "${UFC_SIMDE_INCLUDE_DIR}")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ufc_ppc_generated PRIVATE
        -Wno-c++11-narrowing
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -Wno-unused-function)
endif()

if (MSVC)
    target_compile_options(ufc_ppc_generated PRIVATE /bigobj /MP)
else()
    target_compile_options(ufc_ppc_generated PRIVATE -fno-strict-aliasing)
endif()

add_custom_target(ufc_native_incremental
    DEPENDS ufc_ppc_generated
    COMMENT "Incrementally compiling generated UFC Undisputed 3 PPC translation units")
list(LENGTH UFC_RECOMP_SOURCES UFC_RECOMP_SOURCE_COUNT)

add_executable(ufc3
    "src/native/main.cpp"
    "src/native/compat.cpp"
    "src/native/memory.cpp"
    "src/native/imports/kernel.cpp"
    "src/native/imports/sync.cpp"
    "src/native/imports/pool.cpp"
    "src/native/imports/xam.cpp"
    "src/native/imports/generated_stubs.cpp"
    $<TARGET_OBJECTS:ufc_ppc_generated>)
add_dependencies(ufc3 ufc_ppc_generated)
target_compile_features(ufc3 PRIVATE cxx_std_20)
target_include_directories(ufc3 PRIVATE
    "${UFC_PPC_OUTPUT_DIR}"
    "${UFC_SIMDE_INCLUDE_DIR}")
target_compile_definitions(ufc3 PRIVATE
    UFC_RECOMP_SOURCE_COUNT=${UFC_RECOMP_SOURCE_COUNT})
add_custom_target(ufc_native_imports
    COMMAND powershell -NoProfile -ExecutionPolicy Bypass
        -File "${CMAKE_CURRENT_SOURCE_DIR}/scripts/05_import_inventory.ps1"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Listing implemented and pending native imports")