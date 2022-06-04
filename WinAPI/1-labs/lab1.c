#include <windows.h>
#include <stdio.h>
//прототипы функций которые будут получать сообщения от окон
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwndChild;
HWND textCOORDINATES;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  WNDCLASSA wcl;
  memset(&wcl, 0, sizeof(WNDCLASS));
  wcl.lpszClassName = "MainWin";
  wcl.lpfnWndProc = WndProc;
  wcl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 23);
  wcl.hIcon = LoadIcon(NULL,IDI_ASTERISK);
  wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
  RegisterClassA(&wcl);

  memset(&wcl, 0, sizeof(WNDCLASS));
  wcl.lpszClassName = "ChildWin";
  wcl.lpfnWndProc = WndProcChild;
  wcl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 23);
  wcl.hIcon = LoadIcon(NULL,IDI_ASTERISK);
  wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
  RegisterClassA(&wcl);

  HWND hwnd = CreateWindow("MainWin", "Main", WS_OVERLAPPEDWINDOW|WS_BORDER, 20, 20, 640, 480, NULL, NULL, NULL, NULL);
  HWND textMain = CreateWindow("static", "Click on me", WS_VISIBLE | WS_CHILD , 280, 220, 80, 20, hwnd, NULL, NULL, NULL);
  ShowWindow(hwnd, SW_SHOWNORMAL);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    DispatchMessage(&msg);
  }
  return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_DESTROY:
  {
    PostQuitMessage(0);
  }
  case WM_LBUTTONDOWN:
  {
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    char strx[10];
    char stry[10];
    sprintf(strx, "%d", x);
    sprintf(stry, "%d", y);
    strncat(strx, ";", 1);
    strncat(strx, stry, 10);
     if (FindWindowEx(hwnd,NULL,"ChildWin", "Window with Coordinates") == NULL)
    {
      hwndChild = CreateWindow("ChildWin", "Window with Coordinates", WS_CAPTION | WS_SYSMENU |WS_CHILD ,x, y, 220, 60, hwnd, NULL, NULL, NULL);
      ShowWindow(hwndChild, SW_SHOWNORMAL);
      textCOORDINATES = CreateWindow("static", strx, WS_VISIBLE|WS_CHILD , 0, 0, 220, 20, hwndChild, NULL, NULL, NULL);
    }
    else
    {
      SetWindowText(textCOORDINATES, strx);
      MoveWindow(hwndChild,x,y,220,60,1);
      SetWindowPos(hwndChild,HWND_TOPMOST,0, 0,220,60,SWP_SHOWWINDOW);
    }
    break;
  }
  case WM_RBUTTONDOWN:
  {
    LPRECT rct;
    rct = malloc(sizeof(*rct));
    GetClientRect(hwnd, rct);
    int Xgeneral = rct[0].right;
    int Ygeneral = rct[0].bottom;
    int x = Xgeneral - LOWORD(lParam);
    int y = Ygeneral - HIWORD(lParam);
    char strx[10];
    char stry[10];
    sprintf(strx, "%d", x);
    sprintf(stry, "%d", y);
    strncat(strx, ";", 1);
    strncat(strx, stry, 10);
    if (FindWindowEx(hwnd,NULL,"ChildWin", "Window with Coordinates") == NULL)
    {
      hwndChild = CreateWindow("ChildWin", "Window with Coordinates", WS_CAPTION | WS_SYSMENU |WS_CHILD ,x, y, 220, 60, hwnd, NULL, NULL, NULL);
      ShowWindow(hwndChild, SW_SHOWNORMAL);
      textCOORDINATES = CreateWindow("static", strx, WS_VISIBLE|WS_CHILD , 0, 0, 220, 20, hwndChild, NULL, NULL, NULL);
    }
    else
    {
      SetWindowText(textCOORDINATES, strx);
      MoveWindow(hwndChild,x,y,220,60,1);
      SetWindowPos(hwndChild,HWND_TOPMOST,0, 0,220,60,SWP_SHOWWINDOW);
    }
    break;
  }
  default:
  {
    return DefWindowProcA(hwnd, msg, wParam, lParam);
  }
  }
}

LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  return DefWindowProcA(hwnd, msg, wParam, lParam);
}


