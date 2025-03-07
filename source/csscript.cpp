#include "tsuru/cobra.h"
#include "game/actor/actormgr.h"
#include "game/csscriptmgr.h"

REGISTER_PROFILE(CSScriptLoader, ProfileID::CSScriptLoader);
CSScriptLoader* CSScriptLoader::instance = nullptr;

CSScriptLoader::CSScriptLoader(const ActorBuildInfo* buildInfo)
    : CourseSelectActor(buildInfo)
    , wmscFile(nullptr)
{ }

u32 CSScriptLoader::onCreate() {
    instance = this;
    this->wmscFile = new WMSFile(this->heap);

    return 1;
}

void loadCustomCSScripts() {
    ActorBuildInfo buildInfo = { 0 };

    buildInfo.profile = Profile::get(ProfileID::CSScriptLoader);
    ActorMgr::instance()->create(buildInfo);
}

struct ResultID {
    ResultID() : result(false), id(0) { }
    bool result;
    u32 id;
};

static ResultID customExists(u32 scriptID) {
    ResultID result;

    for (u32 i = 0; i < CSScriptLoader::instance->wmscFile->header.numScripts; i++) {
        if (CSScriptLoader::instance->wmscFile->scriptIDs[i] == scriptID) {
            result.id = i;
            result.result = true;

            return result;
        }
    }

    return result;
}

// TODO: Move these into the real class

u32 getCurrentCommandArg(CSScriptMgr* mgr) {
    u32 out = 0xffffffff;
    if (mgr->isRunningScript()) {
        ResultID result = customExists(mgr->currentScriptID);
        if (result.result) {
            out = CSScriptLoader::instance->wmscFile->scripts[result.id].scriptStart[mgr->currentCommandIndex].arg;
        } else {
            out = WorldMapScriptsTable[mgr->currentScriptID].scriptStart[mgr->currentCommandIndex].arg;
        }
    }

    return out;
}

u32* getCurrentScriptCommandType(CSScriptMgr* mgr, u32* out) {
    if (mgr->isRunningScript()) {
        ResultID result = customExists(mgr->currentScriptID);
        if (result.result) {
            if (out || (out = new u32, out)) {
                *out = CSScriptLoader::instance->wmscFile->scripts[result.id].scriptStart[mgr->currentCommandIndex].type;
            }
        } else {
            if (out || (out = new u32, out)) { // TODO: check
                *out = WorldMapScriptsTable[mgr->currentScriptID].scriptStart[mgr->currentCommandIndex].type;
            }
        }
    } else {
        if (out || (out = new u32, out)) {
            *out = 0;
        }
    }

    return out;
}

u32 getScriptPriority(CSScriptMgr* mgr, u32& tableIndex) {
    ResultID result = customExists(tableIndex);
    if (result.result) {
        return CSScriptLoader::instance->wmscFile->scripts[result.id].priority;
    } else {
        return WorldMapScriptsTable[tableIndex].priority;
    }
}
