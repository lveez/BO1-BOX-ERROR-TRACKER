#include "Window.h"
#include <stdio.h>

const HMENU hmenu_bar = (HMENU)0x1000;
const HMENU hmenu_text = (HMENU)0x1001;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
        case WM_CTLCOLORSTATIC:
            SetBkMode((HDC)w_param, TRANSPARENT);
            SetBkColor((HDC)w_param, RGB(255, 255, 255));
            SetTextColor((HDC)w_param, RGB(0, 0, 0));
            return CreateSolidBrush(RGB(255, 255, 255));
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
        printf("Couldn't register window class. Error: %d\n", GetLastError());
        return 0;
    }

    HWND hwnd = CreateWindow("BETCLASS", "BET", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 100, NULL, NULL, h_instance, NULL);
    if (hwnd == 0)
    {
        printf("Couldn't create window. Error: %d\n", GetLastError());
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}

HWND InitializeProgressBar(HINSTANCE h_instance, HWND hwnd_parent, int max_value)
{
    HWND hwnd = CreateWindowEx(0, PROGRESS_CLASS, (LPSTR)NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 50, 15, 200, 20, hwnd_parent, hmenu_bar, h_instance, NULL);
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
    HWND hwnd = CreateWindow("STATIC", "0 / 24576", SS_CENTER | WS_CHILD | WS_VISIBLE, 50, 40, 200, 20, hwnd_parent, hmenu_text, h_instance, NULL);
    HFONT font = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, "Arial");
    SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);
    return hwnd;
}
