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
};

void ConcatString(char* display_string, short var_count)
{    
    itoa(var_count, display_string, 10);
    strcat(display_string, " / 24576");
}

// for updating window controls and reading the process memory
void MemoryThread(LPVOID p_data)
{
    struct SharedData data = *(struct SharedData*)p_data;

    char display_string[128];

    short var_count_max;
    short var_count;

    DWORD process_id = GetProcessIdByName("BlackOps.exe");
    HANDLE process_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
    DWORD var_count_address = GetVariableCountAddress(process_handle);

    while (true)
    {
        // display the info on the window
        SendMessage(data.hwnd_bar, PBM_SETPOS, var_count_max, 0);
        ConcatString(display_string, var_count_max);
        SetWindowText(data.hwnd_text, display_string);

        // if it connected to the game
        if (process_id)
        {
            SetStatusConnected(data.hwnd_status);
            if (!ReadProcessMemory(process_handle, var_count_address, &var_count, sizeof(short), NULL))
            {
                process_id = 0;
                SetStatusConnecting(data.hwnd_status);
                continue;
            }
            if (var_count > var_count_max) 
                var_count_max = var_count;
            Sleep(50);
        }
        else
        {
            // try to reconnect
            process_id = GetProcessIdByName("BlackOps.exe");
            if (process_id == NULL)
            {
                Sleep(500);
                continue;
            }
            
            process_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
            if (process_handle == NULL)
            {
                Sleep(500);
                process_id = 0;
                continue;
            }

            var_count_address = GetVariableCountAddress(process_handle);
            if (var_count_address == NULL)
            {
                Sleep(500);
                process_id = 0;
                continue;
            }
        }
    }
}

// main thread handles inits windows and handles window messages
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PSTR p_cmd_line, int n_cmd_show)
{
    struct SharedData data;
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
