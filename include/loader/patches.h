#pragma once

#include "loader/common.h"

namespace tloader {
    class Patch {
    public:
        u32 magic;
        u32* addr;
        u16 count;
        u16 dataSize; // in bits
        void* data;
    };
}

#define PATCH_PREFIX _tPatch_

#define _tPatch(addr, bits, ...) \
    _tLoaderSectionStart;        \
    const u##bits TOKENPASTE2(PATCH_PREFIX, TOKENPASTE2(_, addr))[] = { __VA_ARGS__ }; \
    tloader::Patch TOKENPASTE2(PATCH_PREFIX, addr) = {                \
        tloader::DataMagic::Patch, (u32*)addr,                        \
        sizeof(TOKENPASTE2(PATCH_PREFIX, TOKENPASTE2(_, addr))) / sizeof(u##bits), \
        bits, (void*)&TOKENPASTE2(PATCH_PREFIX, TOKENPASTE2(_, addr)) \
    };                                                                \
    _tLoaderSectionEnd

#define tPatch16(addr, ...) _tPatch(addr, 16, __VA_ARGS__)
#define tPatch32(addr, ...) _tPatch(addr, 32, __VA_ARGS__)
#define tPatch64(addr, ...) _tPatch(addr, 64, __VA_ARGS__)
