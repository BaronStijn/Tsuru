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

#define _tPatch(name, addr, bits, ...) \
    _tLoaderSectionStart; \
    const u##bits TOKENPASTE2(name, TOKENPASTE2(_, __LINE__))[] = { __VA_ARGS__ }; \
    tloader::Patch TOKENPASTE2(PATCH_PREFIX, TOKENPASTE2(name, __LINE__)) = { \
        tloader::DataMagic::Patch, (u32*)addr, sizeof(TOKENPASTE2(name, TOKENPASTE2(_, __LINE__))) / sizeof(u##bits), \
        bits, (void*)&TOKENPASTE2(name, TOKENPASTE2(_, __LINE__)) \
    }; \
    _tLoaderSectionEnd

#define tPatch16(name, addr, ...) _tPatch(name, addr, 16, __VA_ARGS__)
#define tPatch32(name, addr, ...) _tPatch(name, addr, 32, __VA_ARGS__)
#define tPatch64(name, addr, ...) _tPatch(name, addr, 64, __VA_ARGS__)
