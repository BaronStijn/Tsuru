#include "log.h"
#include "types.h"
#include "dynlibs/os/functions.h"
#include "dynlibs/fs/functions.h"
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

    PRINT("OSDynLoad_Acquire address: ", LogColor::Yellow, fmt::hex, OS_SPECIFICS->addr_OSDynLoad_Acquire);
    PRINT("OSDynLoad_FindExport address: ", LogColor::Yellow, fmt::hex, OS_SPECIFICS->addr_OSDynLoad_FindExport);

    return;

    //*------------
    //* Step 0: Acquire RPL
    //*------------
    u32 rpl = 0;
    if (OSDynLoad_Acquire("a.rpl", &rpl)) {
        PRINT(LogColor::Red, "Unable to acquire rpl!"); // TODO: Make this print the rpl name
    }

    //*------------
    //* Step 1: Find start of hooks array
    //*------------
    struct {u32 _[4];}* patches;
    if (OSDynLoad_FindExport(rpl, true, _tLoaderSectionNameData, &patches)) {
        PRINT(LogColor::Red, "Unable to find .loaderdata!");
        return;
    }

    PRINT(".loaderdata found at: ", fmt::hex, (u32)patches);

    //*------------
    //* Step 2: Apply hooks
    //*------------
    for (u32 i = 0;; i++) {
        switch (patches[i]._[0]) {
            default: PRINT("Loader complete, applied ", i, " hooks."); return;
                
            case tloader::DataMagic::BranchHook: {
                tloader::BranchHook* hook = (tloader::BranchHook*)&patches[i];

                u32 target = 0;
                OSDynLoad_FindExport(rpl, false, hook->target, &target);

                u32 instr = (target - (u32)hook->source) & 0x03FFFFFC;
                    
                switch (hook->type) {
                    case tloader::BranchHook::Type_b:  instr |= 0x48000000; break;
                    case tloader::BranchHook::Type_bl: instr |= 0x48000001; break;
                    default: PRINT(LogColor::Red, "INVALID HOOK TYPE FOR HOOK AT: ", fmt::hex, (u32)hook->source); continue;
                }

                *hook->source = instr;

                break;
            }

            case tloader::DataMagic::Patch: {
                tloader::Patch* patch = (tloader::Patch*)&patches[i];
                
                for (u16 i = 0; patch->count != 0; i++, patch->count--) {
                    switch (patch->dataSize) {
                        case 8:  *((u8*) patch->addr) = ((u8*) patch->data)[i]; *((u32*)&patch->addr) += 1; break;
                        case 16: *((u16*)patch->addr) = ((u16*)patch->data)[i]; *((u32*)&patch->addr) += 2; break;
                        case 32: *((u32*)patch->addr) = ((u32*)patch->data)[i]; *((u32*)&patch->addr) += 4; break;
                        case 64: *((u64*)patch->addr) = ((u64*)patch->data)[i]; *((u32*)&patch->addr) += 8; break;
                        default: PRINT(LogColor::Red, "INVALID PATCH UNIT SIZE FOR PATCH AT: ", fmt::hex, (u32)patch->addr); break;
                    }
                }
  
                break;
            }
        }
    }
}

void initialize2() {
    SaveMgrSystem::initSystem();
}

tHook(0x11111111, "realfunction100", tloader::BranchHook::Type_b);
tHook(0x11111112, "realfunction200", tloader::BranchHook::Type_bl);
tPatch16(0x33333333, 0x666);
tPatch16(0x33333334, 0x9966, 0x4242);
tPatch32(0x55555555, 0x88888888, 0x99999999);
tPatch64(0x50505050, 0x6666666677ULL);
tPatch64(0x50505051, 0x6666666688ULL);
