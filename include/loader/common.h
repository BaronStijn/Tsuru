#pragma once
#include "types.h"

#define _tLoaderSectionStart                                                                                        \
    CUSTOM_SECTION(data, ".loaderdata");                                                                            \
    CUSTOM_SECTION(rodata, ".loaderconst")

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
