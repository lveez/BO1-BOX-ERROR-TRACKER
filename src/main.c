#include <stdio.h>
#include <stdbool.h>
#include "Memory/Memory.h"
#include "Window/Window.h"

const short max_variable_count = 24576;

typedef struct SharedData
{
    HWND hwnd_text;
    HWND hwnd_bar;
    short* var_count;
};

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

void ConcatString(char* display_string, short var_count)
{    
    itoa(var_count, display_string, 10);
    strcat(display_string, " / 24576");
}

void MemoryThread(LPVOID p_data)
{
    char display_string[128];   

    struct SharedData data = *(struct SharedData*)p_data;

    DWORD process_id = GetProcessIdByName("BlackOps.exe");
    HANDLE p_handle = OpenProcess(PROCESS_VM_READ, TRUE, process_id);
    DWORD var_count_addy = GetVariableCountAddress(p_handle);
    short temp = 0;

    while (true)
    {
        SendMessage(data.hwnd_bar, PBM_SETPOS, *data.var_count, 0);
        ConcatString(display_string, *data.var_count);
        SetWindowText(data.hwnd_text, display_string);
        if (ReadProcessMemory(p_handle, (LPCVOID)var_count_addy, data.var_count, sizeof(short), 0))
        {
            Sleep(50);
            continue;
        }
        char buffer[128];
        itoa(GetLastError(), buffer, 10);
        MessageBox(0, buffer, "Error from ReadProcessMemory.", MB_OK);
        Sleep(10000);
    }
}

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PSTR p_cmd_line, int n_cmd_show)
{
    struct SharedData data;
    short var_count = 0;

    // set up the window
    HWND hwnd_main = InitializeMainWindow(h_instance);
    if (hwnd_main == 0) return MessageBox(NULL, "Error.", "Failed to register window class", MB_OK);
    HWND hwnd_bar = InitializeProgressBar(h_instance, hwnd_main, max_variable_count);
    HWND hwnd_text = InitializeStaticControl(h_instance, hwnd_main);

    data.hwnd_bar = hwnd_bar;
    data.hwnd_text = hwnd_text;
    data.var_count = &var_count;

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MemoryThread, &data, 0, NULL);

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}
