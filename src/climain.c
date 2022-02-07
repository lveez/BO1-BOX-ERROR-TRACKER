#include "datatypes.h"
#include "Memory/Memory.h"
#include "ExternalScrFunctions/ScrVariable.h"
#include <stdio.h>

// address of scr_var_glob is 0x32c8580

int SetCursorToLine(int line_number)
{
    HANDLE std_out;
    COORD cursor_pos = {0, line_number};
	
    std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (std_out == INVALID_HANDLE_VALUE)
	return GetLastError();

    SetConsoleCursorPosition(std_out, cursor_pos);
    return GetLastError();
}

int main()
{
    DWORD process_id = GetProcessIdByName("BlackOps.exe");

    if (!process_id)
        return -1;

    HANDLE p_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);

    int free_vars = 0;
    int box_hits_left = 0;

    while (1)
    {
	SetCursorToLine(0);

        free_vars = GetFreeVariables(p_handle);
        box_hits_left = free_vars / 9.25;

        printf("taken vars: %5d / 65533\n", 65533 - free_vars);
        printf("box hits left: %4d\n", box_hits_left);
	
        Sleep(200);
    }
    return 0;
}
//65533
