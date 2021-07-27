#ifndef MEMORY_H
#define MEMORY_H

// reduces Windows.h header file size
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcessIdByName(char* name);

#endif