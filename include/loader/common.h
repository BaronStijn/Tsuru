#pragma once
#include "types.h"

#define _tLoaderSectionNameData ".loaderdata"
#define _tLoaderSectionNameRodata ".loaderconst"

#define _tLoaderSectionStart                                                                                        \
    CUSTOM_SECTION(data, _tLoaderSectionNameData);                                                                            \
    CUSTOM_SECTION(rodata, _tLoaderSectionNameRodata)

#define _tLoaderSectionEnd                                                                                          \
    CUSTOM_SECTION(data, default);                                                                                  \
    CUSTOM_SECTION(rodata, default)

namespace tloader {
    ENUM_CLASS(DataMagic,
        BranchHook = 0x01C0FFEE,
        PointerHook = 0x02C0FFEE,
        Patch = 0x03C0FFEE,
    );
}
