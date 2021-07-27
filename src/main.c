#include <stdio.h>
#include <stdbool.h>
#include "Memory/Memory.h"
#include "Window/Window.h"

const short max_variable_count = 24576;

struct ProcessParms 
{
    DWORD process_id;
    DWORD variable_count_addy;
    HANDLE p_handle;
    bool looking_for_process;
};

// *(word ptr)(EDX + 0x14)
DWORD GetVariableCountAddress(DWORD process_id)
{
    HANDLE p_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
    int temp;
    if (!ReadProcessMemory(p_handle, (LPCVOID)0x32c8580, &temp, sizeof(int), NULL))
    {
        // GetLastError
        // process has probs been closed
        CloseHandle(p_handle);
        return 0;
    }
    CloseHandle(p_handle);
    return temp + 0x14;
}

void LookForProcess(struct ProcessParms parms)
{
    parms.looking_for_process = true;
    parms.process_id = 0;
    DWORD pid = 0;
    while (pid == 0)
    {
        pid = GetProcessIdByName("BlackOps.exe");
        Sleep(5000);
    }
    parms.variable_count_addy = GetVariableCountAddress(pid);
    parms.p_handle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
    parms.process_id = pid;
    parms.looking_for_process = false;
}

short UpdateTracker(struct ProcessParms parms)
{
    short var_count;
    if (!ReadProcessMemory(parms.p_handle, (LPCVOID)parms.variable_count_addy, &var_count, sizeof(short), NULL))
    {
        // handle is probably gone have to recreate thread somehow -> dont have to close the handle?
    }
    return var_count;
}

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PSTR p_cmd_line, int n_cmd_show)
{
    /*struct ProcessParms *parms;
    parms = malloc(sizeof(*parms));
    if (parms == NULL) return MessageBox(0, "failed malloc", "error", MB_OK);

    parms->looking_for_process = false;
    parms->process_id = 0;
    parms->variable_count_addy = 0;*/

    short var_count = 0;
    char display_string[128];

    // set up the window
    HWND hwnd_main = InitializeMainWindow(h_instance);
    if (hwnd_main == 0) return MessageBox(NULL, "Error.", "Failed to register window class", MB_OK);
    HWND hwnd_bar = InitializeProgressBar(h_instance, hwnd_main, max_variable_count);

    SendMessage(hwnd_bar, PBM_SETBKCOLOR, 0, RGB(218, 218, 241));
    SendMessage(hwnd_bar, PBM_SETBARCOLOR, 0, (RGB(142, 140, 216)));
    SendMessage(hwnd_bar, PBM_SETPOS, 10000, 0);

    HWND hwnd_text = InitializeStaticControl(h_instance, hwnd_main);

    //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LookForProcess, &parms, 0, NULL);
    DWORD pid = GetProcessIdByName("BlackOps.exe");
    DWORD addy = GetVariableCountAddress(pid);
    HANDLE handle = OpenProcess(PROCESS_VM_READ, FALSE, pid);

    MSG Msg;
    while (true)
    {
        if (!GetMessage(&Msg, NULL, 0, 0))
            break;
        ReadProcessMemory(handle, (LPCVOID)addy, &var_count, sizeof(short), NULL);
        SendMessage(hwnd_bar, PBM_SETPOS, var_count, 0);
        itoa(var_count, display_string, 10);
        strcat(display_string, " / 24576");
        SetWindowText(hwnd_text, display_string);
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
        Sleep(500);
    }

    //free(parms);

    return Msg.wParam;
}
