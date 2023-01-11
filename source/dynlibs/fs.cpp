#include "dynlibs/fs/functions.h"
#include "dynlibs/os/functions.h"

EXPORT_DECL(s32, FSInit, void);
EXPORT_DECL(s32, FSShutdown, void);
EXPORT_DECL(s32, FSAddClient, void* client, s32 errHandling);
EXPORT_DECL(s32, FSAddClientEx, void* client, s32 unk_zero_param, s32 errHandling);
EXPORT_DECL(s32, FSDelClient, void* client);
EXPORT_DECL(void, FSInitCmdBlock, void* cmd);
EXPORT_DECL(void*, FSGetCurrentCmdBlock, void* client);
EXPORT_DECL(s32, FSGetMountSource, void* client, void* cmd, s32 type, void* source, s32 errHandling);
EXPORT_DECL(s32, FSMount, void* client, void* cmd, void* source, char* target, u32 bytes, s32 errHandling);
EXPORT_DECL(s32, FSUnmount, void* client, void* cmd, const char* target, s32 errHandling);
EXPORT_DECL(s32, FSRemove, void* client, void* cmd, const char* path, s32 error);
EXPORT_DECL(s32, FSRemoveAsync, void* client, void* cmd, const char* path, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSGetStat, void* client, void* cmd, const char* path, FSStat* stats, s32 errHandling);
EXPORT_DECL(s32, FSGetStatAsync, void* client, void* cmd, const char* path, void* stats, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSRename, void* client, void* cmd, const char* oldPath, const char* newPath, s32 error);
EXPORT_DECL(s32, FSRenameAsync, void* client, void* cmd, const char* oldPath, const char* newPath, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSFlushQuota, void* client, void* cmd, const char* path, s32 error);
EXPORT_DECL(s32, FSFlushQuotaAsync, void* client, void* cmd, const char* path, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSGetFreeSpaceSize, void* client, void* cmd, const char* path, u64* returnedFreeSize, s32 error);
EXPORT_DECL(s32, FSGetFreeSpaceSizeAsync, void* client, void* cmd, const char* path, u64* returnedFreeSize, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSRollbackQuota, void* client, void* cmd, const char* path, s32 error);
EXPORT_DECL(s32, FSRollbackQuotaAsync, void* client, void* cmd, const char* path, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSOpenDir, void* client, void* cmd, const char* path, s32* dh, s32 errHandling);
EXPORT_DECL(s32, FSOpenDirAsync, void* client, void* pCmd, const char* path, s32* handle, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSReadDir, void* client, void* cmd, s32 dh, FSDirEntry* dir_entry, s32 errHandling);
EXPORT_DECL(s32, FSRewindDir, void* client, void* cmd, s32 dh, s32 errHandling);
EXPORT_DECL(s32, FSCloseDir, void* client, void* cmd, s32 dh, s32 errHandling);
EXPORT_DECL(s32, FSChangeDir, void* client, void* cmd, const char* path, s32 errHandling);
EXPORT_DECL(s32, FSChangeDirAsync, void* client, void* cmd, const char* path, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSMakeDir, void* client, void* cmd, const char* path, s32 errHandling);
EXPORT_DECL(s32, FSMakeDirAsync, void* client, void* cmd, const char* path, s32 error, void* asyncParams);
EXPORT_DECL(s32, FSOpenFile, void* client, void* cmd, const char* path, const char* mode, s32* fd, s32 errHandling);
EXPORT_DECL(s32, FSOpenFileAsync, void* client, void* cmd, const char* path, const char* mode, s32* handle, s32 error, const void* asyncParams);
EXPORT_DECL(s32, FSReadFile, void* client, void* cmd, void* buffer, s32 size, s32 count, s32 fd, s32 flag, s32 errHandling);
EXPORT_DECL(s32, FSCloseFile, void* client, void* cmd, s32 fd, s32 errHandling);
EXPORT_DECL(s32, FSFlushFile, void* client, void* cmd, s32 fd, s32 error);
EXPORT_DECL(s32, FSTruncateFile, void* client, void* cmd, s32 fd, s32 error);
EXPORT_DECL(s32, FSGetStatFile, void* client, void* cmd, s32 fd, void* buffer, s32 error);
EXPORT_DECL(s32, FSSetPosFile, void* client, void* cmd, s32 fd, u32 pos, s32 error);
EXPORT_DECL(s32, FSWriteFile, void* client, void* cmd, const void* source, s32 block_size, s32 block_count, s32 fd, s32 flag, s32 error);
EXPORT_DECL(s32, FSBindMount, void* client, void* cmd, char* source, char* target, s32 error);
EXPORT_DECL(s32, FSBindUnmount, void* client, void* cmd, char* target, s32 error);
EXPORT_DECL(s32, FSMakeQuota,  void* client, void* cmd, const char* path,u32 mode, u64 size, s32 errHandling);
EXPORT_DECL(s32, FSMakeQuotaAsync, void* client, void* cmd, const char* path,u32 mode, u64 size, s32 errHandling,const void* asyncParams);
EXPORT_DECL(s32, FSGetCwd, void* client,void* block,char* buffer,u32 bufferSize,u32 flags);

u32 fsHandle = 0;

extern "C" void InitFSFunctionPointers(void) {
    u32* funcPointer = 0;

    OS_FIND_EXPORT(coreinitHandle, FSInit);
    OS_FIND_EXPORT(coreinitHandle, FSShutdown);
    OS_FIND_EXPORT(coreinitHandle, FSAddClient);
    OS_FIND_EXPORT(coreinitHandle, FSAddClientEx);
    OS_FIND_EXPORT(coreinitHandle, FSDelClient);
    OS_FIND_EXPORT(coreinitHandle, FSInitCmdBlock);
    OS_FIND_EXPORT(coreinitHandle, FSGetCurrentCmdBlock);
    OS_FIND_EXPORT(coreinitHandle, FSGetMountSource);
    OS_FIND_EXPORT(coreinitHandle, FSMount);
    OS_FIND_EXPORT(coreinitHandle, FSUnmount);
    OS_FIND_EXPORT(coreinitHandle, FSRename);
    OS_FIND_EXPORT(coreinitHandle, FSRenameAsync);
    OS_FIND_EXPORT(coreinitHandle, FSRemove);
    OS_FIND_EXPORT(coreinitHandle, FSRemoveAsync);
    OS_FIND_EXPORT(coreinitHandle, FSGetStat);
    OS_FIND_EXPORT(coreinitHandle, FSGetStatAsync);
    OS_FIND_EXPORT(coreinitHandle, FSRename);
    OS_FIND_EXPORT(coreinitHandle, FSRenameAsync);
    OS_FIND_EXPORT(coreinitHandle, FSRemove);
    OS_FIND_EXPORT(coreinitHandle, FSRemoveAsync);
    OS_FIND_EXPORT(coreinitHandle, FSFlushQuota);
    OS_FIND_EXPORT(coreinitHandle, FSFlushQuotaAsync);
    OS_FIND_EXPORT(coreinitHandle, FSGetFreeSpaceSize);
    OS_FIND_EXPORT(coreinitHandle, FSGetFreeSpaceSizeAsync);
    OS_FIND_EXPORT(coreinitHandle, FSRollbackQuota);
    OS_FIND_EXPORT(coreinitHandle, FSRollbackQuotaAsync);
    OS_FIND_EXPORT(coreinitHandle, FSOpenDir);
    OS_FIND_EXPORT(coreinitHandle, FSOpenDirAsync);
    OS_FIND_EXPORT(coreinitHandle, FSReadDir);
    OS_FIND_EXPORT(coreinitHandle, FSRewindDir);
    OS_FIND_EXPORT(coreinitHandle, FSCloseDir);
    OS_FIND_EXPORT(coreinitHandle, FSChangeDir);
    OS_FIND_EXPORT(coreinitHandle, FSChangeDirAsync);
    OS_FIND_EXPORT(coreinitHandle, FSMakeDir);
    OS_FIND_EXPORT(coreinitHandle, FSMakeDirAsync);
    OS_FIND_EXPORT(coreinitHandle, FSOpenFile);
    OS_FIND_EXPORT(coreinitHandle, FSOpenFileAsync);
    OS_FIND_EXPORT(coreinitHandle, FSReadFile);
    OS_FIND_EXPORT(coreinitHandle, FSCloseFile);
    OS_FIND_EXPORT(coreinitHandle, FSFlushFile);
    OS_FIND_EXPORT(coreinitHandle, FSTruncateFile);
    OS_FIND_EXPORT(coreinitHandle, FSGetStatFile);
    OS_FIND_EXPORT(coreinitHandle, FSSetPosFile);
    OS_FIND_EXPORT(coreinitHandle, FSWriteFile);
    OS_FIND_EXPORT(coreinitHandle, FSBindMount);
    OS_FIND_EXPORT(coreinitHandle, FSBindUnmount);
    OS_FIND_EXPORT(coreinitHandle, FSMakeQuota);
    OS_FIND_EXPORT(coreinitHandle, FSMakeQuotaAsync);
    OS_FIND_EXPORT(coreinitHandle, FSGetCwd);
}
