#pragma once

#include "types.h"

class BranchHook {
public:
    enum Type {
        Type_b,
        Type_bl,
    };

public:
    u32 magic;
    u32* source;
    const char* target;
    Type type;
};

class PointerHook {
public:
    u32 magic;
    u32* source;
    const char* target;
    u32 _padding;
};

static_assert(sizeof(BranchHook) == sizeof(PointerHook), "BranchHook, PointerHook size mismatch");

#define HOOK_PREFIX _tHook_

#define _tLoaderSectionStart                                                                                        \
    CUSTOM_SECTION(data, ".loaderdata");                                                                            \
    CUSTOM_SECTION(rodata, ".loaderconst")

#define _tLoaderSectionEnd                                                                                          \
    CUSTOM_SECTION(data, default);                                                                                  \
    CUSTOM_SECTION(rodata, default)

#define tyHook(name, source, target, type) \
    _tLoaderSectionStart;                                                                                           \
    BranchHook TOKENPASTE2(HOOK_PREFIX, TOKENPASTE2(name, __LINE__)) = { 0x01C0FFEE, (u32*)source, target, type };  \
    _tLoaderSectionEnd

#define tyPointer(name, source, target)                                                                             \
    _tLoaderSectionStart;                                                                                           \
    PointerHook TOKENPASTE2(HOOK_PREFIX, TOKENPASTE2(name, __LINE__)) = { 0x02C0FFEE, (u32*)source, target, 0 };    \
    _tLoaderSectionEnd
