#include <Windows.h>

#include <sstream>

#include "resource.h"

#include "wotcheater.h"

#include "Commctrl.h"

WOTCheater g_wotcheater;
HINSTANCE g_Instance = NULL;
HWND g_StatusWnd = NULL;
HBRUSH g_hbrWhite = NULL;

void enableTundra()
{

    g_wotcheater.setState(true);
}

void disableTundra()
{
    g_wotcheater.setState(false);
}

void setStatusMessage(const wchar_t* msg)
{
    SetWindowText(g_StatusWnd, msg);
}

void changeTundraState()
{
    g_wotcheater.setState(!g_wotcheater.getState());
}

void updateCheatStatus(bool status)
{
    std::wstring msg;

    if (g_wotcheater.queryCheatStatus())
    {
        msg = L"World Of Tanks обнаружен. ";
    }
    else
    {
        msg = L"World Of Tanks не обнаружен. ";
    }

    if (g_wotcheater.getState())
    {
        msg += L"[Тундра включена!]";
    }
    else
    {
        msg += L"[Тундра не включена!]";
    }

    setStatusMessage(msg.c_str());
}

HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE
                      hinst, int cParts)
{
    HWND hwndStatus;
    RECT rcClient;
    HLOCAL hloc;
    PINT paParts;
    int i, nWidth;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the status bar.
    hwndStatus = CreateWindowEx(
        0,                       // no extended styles
        STATUSCLASSNAME,         // name of status bar class
        (PCTSTR) NULL,           // no text when first created
        WS_CHILD | WS_VISIBLE,   // creates a visible child window
        0, 0, 0, 0,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    // Get the coordinates of the parent window's client area.
    GetClientRect(hwndParent, &rcClient);

    // Allocate an array for holding the right edge coordinates.
    hloc = LocalAlloc(LHND, sizeof(int) * cParts);
    paParts = (PINT) LocalLock(hloc);

    // Calculate the right edge coordinate for each part, and
    // copy the coordinates to the array.
    nWidth = rcClient.right / cParts;
    int rightEdge = nWidth;
    for (i = 0; i < cParts; i++) { 
       paParts[i] = rightEdge;
       rightEdge += nWidth;
    }

    // Tell the status bar to create the window parts.
    SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) cParts, (LPARAM)
               paParts);

    // Free the array, and return.
    LocalUnlock(hloc);
    LocalFree(hloc);

    ::SetWindowTextW(hwndStatus, L"asasd");

    return hwndStatus;
}  


INT_PTR CALLBACK DialogProc (HWND hwnd, 
                          UINT message, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:

        g_StatusWnd = DoCreateStatusBar(hwnd, 100, g_Instance, 1);

        ::SetTimer(hwnd, 0, 100, NULL);

        g_hbrWhite = (HBRUSH)GetStockObject(WHITE_BRUSH); 

        g_wotcheater.setState(true);

        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_TRUNKSMODE:
            g_wotcheater.setTrunkMode(!g_wotcheater.isTrunkMode());
            break;
        case IDC_SNIPERMODE:
            g_wotcheater.setSniperMode(!g_wotcheater.isSniperMode());
            break;
        }
        return TRUE;
    case WM_ERASEBKGND:
    {
        RECT rc;
        HDC hdc = (HDC) wParam; 
        GetClientRect(hwnd, &rc); 
        SetMapMode(hdc, MM_ANISOTROPIC); 
        FillRect(hdc, &rc, g_hbrWhite); 
        
        return 1L; 
    }
    case WM_TIMER:
        {
            if (::GetAsyncKeyState(VK_F2) != 0)
            {
                Beep(800, 200);
                changeTundraState();
            }

            updateCheatStatus(g_wotcheater.queryCheatStatus());
        }
        break;
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORDLG:
        return (INT_PTR)g_hbrWhite;   
    case WM_DESTROY:
        PostQuitMessage(0);
        return TRUE;
    case WM_CLOSE:
        DestroyWindow (hwnd);
        return TRUE;
    }
    return FALSE;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_Instance = hInstance;

    HWND hDlg = CreateDialogW(hInstance, MAKEINTRESOURCE (IDD_TUNDRA), 0, DialogProc);
    if (hDlg==NULL)
    {
        std::wostringstream errstrm;
        errstrm << "Error: " << GetLastError ();

        MessageBoxW (0, errstrm.str().c_str(), L"Error", MB_ICONEXCLAMATION | MB_OK);

        return 1;
    }

    ::ShowWindow(hDlg, SW_SHOW);

    MSG  msg;
    int status;
    while ((status = GetMessage(&msg, 0, 0, 0))!=0)
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    return 0;
}