#include "game/task/coursetask.h"
#include "game/actor/stage/stageactor.h"
#include "game/task/taskmgr.h"
#include "tsuru/debug/debugscene.h"
#include "log.h"

class TaskChanger : public StageActor {
    SEAD_RTTI_OVERRIDE_IMPL(TaskChanger, StageActor);

public:
    TaskChanger(const ActorBuildInfo* buildInfo);
    virtual ~TaskChanger() { }

    u32 onCreate() override;
};

REGISTER_PROFILE(TaskChanger, ProfileID::TaskChanger);

TaskChanger::TaskChanger(const ActorBuildInfo* buildInfo)
    : StageActor(buildInfo)
{ }

u32 TaskChanger::onCreate() {
    sead::TaskClassID taskClassID;
    taskClassID.type = sead::TaskClassID::Type_Factory;
    taskClassID.id.factory = &DebugScene::construct; //! Set a target task!

    bool t = TaskMgr::instance()->changeTask(CourseTask::instance(), taskClassID, 0, 0);

    PRINT("TaskMgr says: ", t);

    return 0;
}
