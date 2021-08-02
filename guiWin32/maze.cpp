#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "maze-maker/MazeApp.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

UINT32 REFRESH_WINDOW_ID = USER_TIMER_MINIMUM + 1;

class MazeGui : public MazeApp
{
public:
    MazeGui(Size size, Location start, Location end, int seed, int maxDistance, int pixelsPerCell)
        : MazeApp(size, start, end, seed, maxDistance, pixelsPerCell)
    {
    }
protected:
    virtual void onCellChanged(Location l)
    {
    }
    virtual void onPathAdded(Location begin, Location end)
    {
    }
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    //MazeGui gui;

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Maze Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.cbWndExtra = sizeof(MazeGui*);

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"https://github.com/mrsalt/maze-maker",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    SetTimer(hwnd, REFRESH_WINDOW_ID, 20, NULL);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static bool go = false;

    switch (uMsg)
    {
    case WM_SIZE: {
        LONG_PTR windowPtr = GetWindowLongPtr(hwnd, 0);
        if (windowPtr != NULL) {
            go = false;
            MazeGui* gui = (MazeGui*)windowPtr;
            delete gui;
            SetWindowLongPtr(hwnd, 0, (LONG_PTR)NULL);
        }
        return 0;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    case WM_TIMER: {
        if (wParam == REFRESH_WINDOW_ID) {
            RECT rect;
            GetClientRect(hwnd, &rect);
            InvalidateRect(hwnd, &rect, FALSE);
        }
        return 0;
    }
    case WM_LBUTTONDOWN: {
        go = true;
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        LONG_PTR windowPtr = GetWindowLongPtr(hwnd, 0);
        MazeGui* gui;
        if (windowPtr == NULL) {
            int cellSize = 20;
            Size size{ (ps.rcPaint.right - ps.rcPaint.left) / cellSize, (ps.rcPaint.bottom - ps.rcPaint.top) / cellSize };
            Location start{ 0, 0 };
            Location end{ size.width - 1, size.height - 1 };
            int seed = 0;
            gui = new MazeGui(size, start, end, seed, 10, cellSize);
            SetWindowLongPtr(hwnd, 0, (LONG_PTR) gui);
        }
        else {
            gui = (MazeGui*) windowPtr;
        }

        if (go && gui->iterate()) {
            MazeRenderer& renderer = gui->getRenderer();
            renderer.render(false);
                
            BITMAP bm;
            bm.bmType = 0;
            bm.bmWidth = renderer.getImageSize().width;
            bm.bmHeight = renderer.getImageSize().height;
            bm.bmWidthBytes = renderer.getImageStride();
            bm.bmPlanes = 1;
            bm.bmBitsPixel = 32;
            bm.bmBits = renderer.getImageBits();
            HBITMAP bitmap = CreateBitmapIndirect(&bm);
            HDC srcDC = CreateCompatibleDC(hdc);
            HGDIOBJ oldBitmap = SelectObject(srcDC, bitmap);

            BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, srcDC, 0, 0, SRCCOPY);

            SelectObject(srcDC, oldBitmap);
            DeleteObject(bitmap);
            DeleteDC(srcDC);
        }

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
