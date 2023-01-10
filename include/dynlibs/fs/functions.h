#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlibs/fs/types.h"

void InitFSFunctionPointers(void);

extern s32 (*FSInit)(void);
extern s32 (*FSShutdown)(void);
extern s32 (*FSAddClient)(void* client, s32 errHandling);
extern s32 (*FSAddClientEx)(void* client, s32 unk_zero_param, s32 errHandling);
extern s32 (*FSDelClient)(void* client);
extern void (*FSInitCmdBlock)(void* cmd);
extern void* (*FSGetCurrentCmdBlock)(void* client);
extern s32 (*FSGetMountSource)(void* client, void* cmd, s32 type, void* source, s32 errHandling);

extern s32 (*FSMount)(void* client, void* cmd, void* source, char* target, u32 bytes, s32 errHandling);
extern s32 (*FSUnmount)(void* client, void* cmd, const char* target, s32 errHandling);
extern s32 (*FSRename)(void* client, void* cmd, const char* oldPath, const char* newPath, s32 error);
extern s32 (*FSRenameAsync)(void* client, void* cmd, const char* oldPath, const char* newPath, s32 error, void* asyncParams);
extern s32 (*FSRemove)(void* client, void* cmd, const char* path, s32 error);
extern s32 (*FSRemoveAsync)(void* client, void* cmd, const char* path, s32 error, void* asyncParams);

extern s32 (*FSGetStat)(void* client, void* cmd, const char* path, FSStat* stats, s32 errHandling);
extern s32 (*FSGetStatAsync)(void* client, void* cmd, const char* path, void* stats, s32 error, void* asyncParams);
extern s32 (*FSRename)(void* client, void* cmd, const char* oldPath, const char* newPath, s32 error);
extern s32 (*FSRenameAsync)(void* client, void* cmd, const char* oldPath, const char* newPath, s32 error, void* asyncParams);
extern s32 (*FSRemove)(void* client, void* cmd, const char* path, s32 error);
extern s32 (*FSRemoveAsync)(void* client, void* cmd, const char* path, s32 error, void* asyncParams);
extern s32 (*FSFlushQuota)(void* client, void* cmd, const char* path, s32 error);
extern s32 (*FSFlushQuotaAsync)(void* client, void* cmd, const char* path, s32 error, void* asyncParams);
extern s32 (*FSGetFreeSpaceSize)(void* client, void* cmd, const char* path, u64* returnedFreeSize, s32 error);
extern s32 (*FSGetFreeSpaceSizeAsync)(void* client, void* cmd, const char* path, u64* returnedFreeSize, s32 error, void* asyncParams);
extern s32 (*FSRollbackQuota)(void* client, void* cmd, const char* path, s32 error);
extern s32 (*FSRollbackQuotaAsync)(void* client, void* cmd, const char* path, s32 error, void* asyncParams);

extern s32 (*FSOpenDir)(void* client, void* cmd, const char* path, s32* dh, s32 errHandling);
extern s32 (*FSOpenDirAsync)(void* client, void* pCmd, const char* path, s32* handle, s32 error, void* asyncParams);
extern s32 (*FSReadDir)(void* client, void* cmd, s32 dh, FSDirEntry* dir_entry, s32 errHandling);
extern s32 (*FSRewindDir)(void* client, void* cmd, s32 dh, s32 errHandling);
extern s32 (*FSCloseDir)(void* client, void* cmd, s32 dh, s32 errHandling);
extern s32 (*FSChangeDir)(void* client, void* cmd, const char* path, s32 errHandling);
extern s32 (*FSChangeDirAsync)(void* client, void* cmd, const char* path, s32 error, void* asyncParams);
extern s32 (*FSMakeDir)(void* client, void* cmd, const char* path, s32 errHandling);
extern s32 (*FSMakeDirAsync)(void* client, void* cmd, const char* path, s32 error, void* asyncParams);

extern s32 (*FSOpenFile)(void* client, void* cmd, const char* path, const char* mode, s32* fd, s32 errHandling);
extern s32 (*FSOpenFileAsync)(void* client, void* cmd, const char* path, const char* mode, s32* handle, s32 error, const void* asyncParams);
extern s32 (*FSReadFile)(void* client, void* cmd, void* buffer, s32 size, s32 count, s32 fd, s32 flag, s32 errHandling);
extern s32 (*FSCloseFile)(void* client, void* cmd, s32 fd, s32 errHandling);

extern s32 (*FSFlushFile)(void* client, void* cmd, s32 fd, s32 error);
extern s32 (*FSTruncateFile)(void* client, void* cmd, s32 fd, s32 error);
extern s32 (*FSGetStatFile)(void* client, void* cmd, s32 fd, void* buffer, s32 error);
extern s32 (*FSSetPosFile)(void* client, void* cmd, s32 fd, u32 pos, s32 error);
extern s32 (*FSWriteFile)(void* client, void* cmd, const void* source, s32 block_size, s32 block_count, s32 fd, s32 flag, s32 error);

extern s32 (*FSBindMount)(void* client, void* cmd, char* source, char* target, s32 error);
extern s32 (*FSBindUnmount)(void* client, void* cmd, char* target, s32 error);

extern s32 (*FSMakeQuota)( void* client, void* cmd, const char* path,u32 mode, u64 size, s32 errHandling);
extern s32 (*FSMakeQuotaAsync)(void* client, void* cmd, const char* path,u32 mode, u64 size, s32 errHandling,const void* asyncParams);

extern s32 (*FSGetCwd)(void* client,void* block,char* buffer,u32 bufferSize,u32 flags);

#ifdef __cplusplus
}
#endif
