#include "Window.h"
#include <stdio.h>

const HMENU hmenu_bar = (HMENU)0x1000;
const HMENU hmenu_text = (HMENU)0x1001;
const HMENU hmenu_status = (HMENU)0x1002;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
        case WM_CTLCOLORSTATIC:
            SetBkMode((HDC)w_param, TRANSPARENT);
            SetBkColor((HDC)w_param, RGB(255, 255, 255));
            SetTextColor((HDC)w_param, RGB(0, 0, 0));
            return GetSysColorBrush(COLOR_WINDOW);
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}

HWND InitializeMainWindow(HINSTANCE h_instance)
{
    char error_buffer[128];

    WNDCLASS wnd_class;
    wnd_class.style = CS_HREDRAW | CS_VREDRAW;
    wnd_class.lpfnWndProc = WindowProc;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = h_instance;
    wnd_class.hIcon = NULL;
    wnd_class.hCursor = NULL;
    wnd_class.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wnd_class.lpszMenuName = NULL;
    wnd_class.lpszClassName = "BETCLASS";

    if (!RegisterClass(&wnd_class))
    {
        itoa(GetLastError(), error_buffer, 10);
        MessageBox(0, error_buffer, "Couldn't register window class.", MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindow("BETCLASS", "BET", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 100, NULL, NULL, h_instance, NULL);
    if (hwnd == 0)
    {
        itoa(GetLastError(), error_buffer, 10);
        MessageBox(0, error_buffer, "Couldn't create main window.", MB_OK);
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}

HWND InitializeProgressBar(HINSTANCE h_instance, HWND hwnd_parent, int max_value)
{
    char error_buffer[128];
    HWND hwnd = CreateWindowEx(0, PROGRESS_CLASS, (LPSTR)NULL, WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 50, 15, 200, 20, hwnd_parent, hmenu_bar, h_instance, NULL);
    if (hwnd == 0)
    {        
        itoa(GetLastError(), error_buffer, 10);
        MessageBox(0, error_buffer, "Couldn't create progress bar window.", MB_OK);
        return 0;
    }
    // remove border
    long style = GetWindowLong(hwnd, GWL_EXSTYLE);
    style &= ~WS_EX_STATICEDGE;
    SetWindowLong(hwnd, GWL_EXSTYLE, style);
    //
    SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, max_value));
    SendMessage(hwnd, PBM_SETBKCOLOR, 0, RGB(218, 218, 241));
    SendMessage(hwnd, PBM_SETBARCOLOR, 0, (RGB(142, 140, 216)));
    return hwnd;
}

HWND InitializeStaticControl(HINSTANCE h_instance, HWND hwnd_parent)
{
    char error_buffer[128];
    HWND hwnd = CreateWindow("STATIC", "0 / 24576", SS_CENTER | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE, 50, 40, 200, 20, hwnd_parent, hmenu_text, h_instance, NULL);
    if (hwnd == 0)
    {        
        itoa(GetLastError(), error_buffer, 10);
        MessageBox(0, error_buffer, "Couldn't create text window.", MB_OK);
        return 0;
    }
    HFONT font = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "Arial");
    SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);
    return hwnd;
}

HWND InitializeStatusControl(HINSTANCE h_instance, HWND hwnd_parent)
{
    char error_buffer[128];
    HWND hwnd = CreateWindow("STATIC", "connecting...", SS_LEFT | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE, 5, 50, 100, 18, hwnd_parent, hmenu_status, h_instance, NULL);
    if (hwnd == 0)
    {        
        itoa(GetLastError(), error_buffer, 10);
        MessageBox(0, error_buffer, "Couldn't create status window.", MB_OK);
        return 0;
    }
    HFONT font = CreateFont(16, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "Arial");
    SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);
    SetWindowPos(hwnd, HWND_TOP, 5, 50, 100, 18, 0);
    return hwnd;
}

void SetStatusConnecting(HWND hwnd)
{
    SetWindowText(hwnd, "connecting...");
}

void SetStatusConnected(HWND hwnd)
{
    SetWindowText(hwnd, "");
}
