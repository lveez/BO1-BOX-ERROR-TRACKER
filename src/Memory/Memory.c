#include "Memory.h"

DWORD GetProcessIdByName(char* name)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    if (Process32First(snapshot, &entry))
    {
        do 
        {
            if (strcmp(name, entry.szExeFile) == 0)
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry));
    }
    return 0;
}



DWORD GetVariableCountAddress(HANDLE p_handle)
{
    int temp;
    if (!ReadProcessMemory(p_handle, (LPCVOID)0x32c8580, &temp, sizeof(int), NULL))
    {
        // GetLastError
        // process has probs been closed
        return 0;
    }
    return temp + 0x14;
}

