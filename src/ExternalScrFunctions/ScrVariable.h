#ifndef SCRVARIABLE_H
#define SCRVARIABLE_H

#ifndef WIN32_LEAN_AND_MEAN
#define WN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <stdio.h>
#include "../datatypes.h"

// an external version of Scr_DumpScriptThreads, only for server script instance
void DumpScriptThreadsExternal(HANDLE p_handle);
int GetFreeVariables(HANDLE p_handle);
int GetTakenVariables(HANDLE p_handle);
int GetStackVariables(HANDLE p_handle);
int GetAnimVariables(HANDLE p_handle);
int GetFunctions(HANDLE p_handle);
/*int GetObjects(HANDLE p_handle);
int GetEntities(HANDLE p_handle);
int GetThreads(HANDLE p_handle);*/

#endif
