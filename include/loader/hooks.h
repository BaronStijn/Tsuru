#pragma once

#include "loader/common.h"

namespace tloader {
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
}

static_assert(sizeof(tloader::BranchHook) == sizeof(tloader::PointerHook), "BranchHook <-> PointerHook size mismatch");

#define HOOK_PREFIX _tHook_

#define tHook(name, source, target, type) \
    _tLoaderSectionStart;                                                                                           \
    tloader::BranchHook TOKENPASTE2(HOOK_PREFIX, TOKENPASTE2(name, __LINE__)) = { tloader::DataMagic::BranchHook, (u32*)source, target, type };  \
    _tLoaderSectionEnd

#define tPointer(name, source, target)                                                                             \
    _tLoaderSectionStart;                                                                                           \
    tloader::PointerHook TOKENPASTE2(HOOK_PREFIX, TOKENPASTE2(name, __LINE__)) = { tloader::DataMagic::PointerHook, (u32*)source, target, 0 };    \
    _tLoaderSectionEnd
