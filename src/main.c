#include <stdio.h>
#include <stdbool.h>
#include "Memory/Memory.h"
#include "Window/Window.h"

const short max_variable_count = 24576;

typedef struct SharedData
{
    HWND hwnd_text;
    HWND hwnd_status;
    HWND hwnd_bar;
    HWND hwnd_main;
    short* var_count;
};

void ConcatString(char* display_string, short var_count)
{    
    itoa(var_count, display_string, 10);
    strcat(display_string, " / 24576");
}

// for updating window controls and reading the process memory
void MemoryThread(LPVOID p_data)
{
    char display_string[128];      
    char error_buffer[128];
    DWORD error;
    int i = 0;

    struct SharedData data = *(struct SharedData*)p_data;

    DWORD process_id = GetProcessIdByName("BlackOps.exe");
    HANDLE p_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
    DWORD var_count_addy = GetVariableCountAddress(p_handle);

    while (true)
    {
        SendMessage(data.hwnd_bar, PBM_SETPOS, *data.var_count, 0);
        if (i % 5 == 0)
        {
            ConcatString(display_string, *data.var_count);
            SetWindowText(data.hwnd_text, display_string);
            i = 0;
        }
        i++;

        if (process_id != 0)
        {
            SetStatusConnected(data.hwnd_status);
            // if ReadProcessMemory succeeds continue else rescan for game
            if (ReadProcessMemory(p_handle, (LPCVOID)var_count_addy, data.var_count, sizeof(short), 0))
            {
                error = GetLastError();
                Sleep(50);
                continue;
            }
            SetStatusConnecting(data.hwnd_status);
            process_id = 0;
            error = GetLastError();
        }

        if (error == ERROR_PARTIAL_COPY || error == ERROR_INVALID_HANDLE || error == NO_ERROR)
        {
            process_id = GetProcessIdByName("BlackOps.exe");
            if (process_id == 0) continue;

            p_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
            if (p_handle == INVALID_HANDLE_VALUE)
            {
                process_id = 0;
                continue;
            }

            var_count_addy = GetVariableCountAddress(p_handle);
        }
        else
        {
            itoa(error, error_buffer, 10);
            MessageBox(NULL, error_buffer, "Unhandled Error", MB_OK);
        }
        /*itoa(GetLastError(), error_buffer, 10);
        MessageBox(0, error_buffer, "Error from ReadProcessMemory.", MB_OK);*/
        Sleep(1000);
    }
}

// main thread handles inits windows and handles window messages
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PSTR p_cmd_line, int n_cmd_show)
{
    struct SharedData data;
    short var_count = 0;   
    char error_buffer[128];

    // set up the window
    HWND hwnd_main = InitializeMainWindow(h_instance);
    HWND hwnd_bar = InitializeProgressBar(h_instance, hwnd_main, max_variable_count);
    HWND hwnd_text = InitializeStaticControl(h_instance, hwnd_main);
    HWND hwnd_status = InitializeStatusControl(h_instance, hwnd_main);

    if (hwnd_main == 0 || hwnd_bar == 0 || hwnd_text == 0 || hwnd_status == 0)
        return 0;

    data.hwnd_bar = hwnd_bar;
    data.hwnd_text = hwnd_text;
    data.var_count = &var_count;
    data.hwnd_status = hwnd_status;
    data.hwnd_main = hwnd_main;

    if (CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MemoryThread, &data, 0, NULL) == NULL)
    {
        itoa(GetLastError(), error_buffer, 10);
        MessageBox(0, error_buffer, "Failed to create thread.", MB_OK);
        return 0;
    }

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}
