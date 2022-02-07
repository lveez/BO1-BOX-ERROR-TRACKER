#ifndef THIS_WINDOW_H
#define THIS_WINDOW_H

#include <Windows.h>
#include <CommCtrl.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HWND InitializeMainWindow(HINSTANCE);
HWND InitializeProgressBar(HINSTANCE, HWND, int);
HWND InitializeStaticControl(HINSTANCE, HWND);
HWND InitializeStatusControl(HINSTANCE, HWND);
void SetStatusConnecting(HWND);
void SetStatusConnected(HWND);

#endif