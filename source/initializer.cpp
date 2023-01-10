#include "log.h"
#include "types.h"
#include "dynlibs/os/functions.h"
#include "dynlibs/gx2/functions.h"
#include "tsuru/save/system/savemgrsystem.h"
#include "loader/hooks.h"
#include "loader/patches.h"

/*
    ______
   /_  __/______  _________  __
    / / / ___/ / / / ___/ / / /
   / / (___ ) /_/ / /  / /_/ /
  /_/ /____/\__,_/_/   \__,_/

      _              __             _                       _           _
     | |__ _  _     / /  __ ____ _ (_) __  __ ____ __   ___| |_    __ _| |
     | '_ \ || |   / /__/ // /  ' \/ / _ \/ // /\ \ /  / -_)  _|  / _` | |_
     |_.__/\_, |  /____/\_,_/_/_/_/_/_//_/\_, //_\_\   \___|\__|  \__,_|_(_)
            |__/                         /___/
*/

// staticInit array
extern "C" funcPtr _ctors[];

// RPL loading dependencies
OsSpecifics osSpecifics;
extern u32 BLOSDynLoad_Acquire;
extern u32 BOSDynLoad_FindExport;
extern u32 coreinitHandle;

void initialize() {
    // Duplicate call check
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    // Call staticInit functions from _ctors array
    for (s32 i = 0; _ctors[i]; i++)
        (*_ctors[i])();

    // Set OSDynLoad_Acquire and OSDynLoad_FindExport
    OS_SPECIFICS->addr_OSDynLoad_Acquire    = (u32)(BLOSDynLoad_Acquire   & 0x03FFFFFC);
    OS_SPECIFICS->addr_OSDynLoad_FindExport = (u32)(BOSDynLoad_FindExport & 0x03FFFFFC);

    if (!(BLOSDynLoad_Acquire & 2))
        OS_SPECIFICS->addr_OSDynLoad_Acquire    += (u32)&BLOSDynLoad_Acquire;
    if (!(BOSDynLoad_FindExport & 2))
        OS_SPECIFICS->addr_OSDynLoad_FindExport += (u32)&BOSDynLoad_FindExport;

    // Init RPL libraries
    InitOSFunctionPointers();
    InitGX2FunctionPointers();

    $(void)()(
        //*------------
        //* Step 1: Find start of hooks array
        //*------------
        char hookName[12] = { 0 };
        u32 firstHookAddr = 0xFFFFFFFF;
        //u32 numHooks = 0;

        for (u32 i = 0; ; i++) {
            __os_snprintf(hookName, 12, "_tHook_%04d", i);

            PRINT("Searching for hook: ", hookName);

            u32 addr;
            if (OSDynLoad_FindExport(coreinitHandle, true, hookName, &addr)) {
                PRINT("Stop!");
                //numHooks = i;
                break;
            }

            if (addr < firstHookAddr) {
                firstHookAddr = addr;
            }
        }

        if (firstHookAddr == 0xFFFFFFFF) {
            PRINT("No hooks found!");
            return;
        }

        PRINT("First hook found at: ", fmt::hex, firstHookAddr);

        //*------------
        //* Step 2: Apply hooks
        //*------------
        tloader::BranchHook* hooks = (tloader::BranchHook*)firstHookAddr;
        for (u32 i = 0; i < 2; i++) {
            const tloader::BranchHook& hook = hooks[i];

            u32 target = 0;
            OSDynLoad_FindExport(coreinitHandle, false, hook.target, &target);
        
            u32 instr = (target - (u32)hook.source) & 0x03FFFFFC;

            switch (hook.type) {
                case tloader::BranchHook::Type_b:  instr |= 0x48000000; break;
                case tloader::BranchHook::Type_bl: instr |= 0x48000001; break;
                default: PRINT(LogColor::Red, "INVALID HOOK TYPE FOR HOOK AT: ", fmt::hex, (u32)hook.source); continue;
            }

            *hook.source = instr;
        }
    ) applyHooks;

    applyHooks();

    PRINT("OSDynLoad_Acquire address: ", LogColor::Yellow, fmt::hex, OS_SPECIFICS->addr_OSDynLoad_Acquire);
    PRINT("OSDynLoad_FindExport address: ", LogColor::Yellow, fmt::hex, OS_SPECIFICS->addr_OSDynLoad_FindExport);

    PRINT(LogColor::Green, "Custom code initialization complete!");
}

void initialize2() {
    SaveMgrSystem::initSystem();
}

tHook(TestHook1, 0x11111111, "realfunction100", tloader::BranchHook::Type_b); tHook(TestHook1, 0x11111112, "realfunction200", tloader::BranchHook::Type_bl);
tPatch16(TestPatch1, 0x33333333, 0x666); tPatch16(TestPatch1, 0x33333334, 0x9966, 0x4242);
tPatch32(TestPatch2, 0x55555555, 0x88888888, 0x99999999);
tPatch64(TestPatch2, 0x50505050, 0x6666666677ULL);
tPatch64(TestPatch2, 0x50505051, 0x6666666688ULL);
