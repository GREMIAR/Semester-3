#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define button_id1 1
#define button_id2 2
#define button_id3 3
#define button_id7 777
#define edit_id4 4
#define edit_id5 5
#define edit_id6 6
void RegClass(WNDPROC,LPCTSTR);
HWND hwndMain;
HINSTANCE DLLGREBIAR;
HWND textMain;
HDC hdc;
enum stateDraw{
  Stop,
  LineDraw,
  RectangleDraw,
  CirclDraw
};
HWND edit1;
HWND edit2;
HWND edit3;
enum stateDraw DrawFigures;
POINT coordinates[2];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(FrameWndProc,"MainWin");
  hwndMain = CreateWindow("MainWin", "Main Window", WS_OVERLAPPEDWINDOW, 40, 40, 1100, 720, NULL, NULL, NULL, NULL);
  ShowWindow(hwndMain, SW_SHOWNORMAL);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

void RegClass(WNDPROC Proc,LPCTSTR szName)
{
  WNDCLASSA wcl;
  memset(&wcl, 0, sizeof(WNDCLASS));
  wcl.lpszClassName = szName;
  wcl.lpfnWndProc = Proc;
  wcl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 23);
  wcl.hIcon = LoadIcon(NULL,IDI_ASTERISK);
  wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
  RegisterClassA(&wcl);
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_CREATE:
    {
      hdc = GetWindowDC(hwnd);
      HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
      HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
      SelectObject(hdc, pen);
      SelectObject(hdc, brush);
      DLLGREBIAR = LoadLibrary("drawGREBIAR.dll"); 
      HWND button1 = CreateWindow("button", "LINE", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 40, 20, 250, 40, hwnd, (HMENU)button_id1, NULL, NULL);
      HWND button2 = CreateWindow("button", "RECTANGLE", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 330, 20, 250, 40, hwnd, (HMENU)button_id2, NULL, NULL);
      HWND button3 = CreateWindow("button", "CIRCLE", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 620, 20, 250, 40, hwnd, (HMENU)button_id3, NULL, NULL);
      edit1 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 900, 38, 30, 20, hwnd, (HMENU)edit_id4, NULL, NULL);
      edit2 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 950, 38, 30, 20, hwnd, (HMENU)edit_id5, NULL, NULL);
      edit3 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 1000, 38, 30, 20, hwnd, (HMENU)edit_id6, NULL, NULL);
      HWND textMain1 = CreateWindow("static", "Color RGB", WS_VISIBLE | WS_CHILD, 935, 15, 80, 20, hwnd, NULL, NULL, NULL);
      HWND R = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, 885, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND B = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, 935, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND G = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, 985, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND button4 = CreateWindow("button", "OK", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 1040, 38, 30, 20, hwnd, (HMENU)button_id7, NULL, NULL);
      break;
    }
    case WM_LBUTTONDOWN:
    {
      int x = LOWORD(lParam)+5;
      int y = HIWORD(lParam)+30;
      if(DrawFigures==LineDraw)
      {
        void (*DllFunc) (HDC hdc,int x,int y);
        DllFunc=(void (*) (HDC hdc,int x,int y)) GetProcAddress( DLLGREBIAR,"SettingCursor");
        DllFunc(hdc,x,y);
      }
      else if(DrawFigures==RectangleDraw)
      {
        coordinates[0].x=x;
        coordinates[0].y=y;
      }
      else if(DrawFigures==CirclDraw)
      {
        coordinates[0].x=x;
        coordinates[0].y=y;
      }
      break;
    }
    case WM_LBUTTONUP:
    {
      int x = LOWORD(lParam)+5;
      int y = HIWORD(lParam)+30;
      if(DrawFigures==LineDraw)
      {
        void (*DllFunc) (HDC hdc,int x,int y);
        DllFunc=(void (*) (HDC hdc,int x,int y)) GetProcAddress( DLLGREBIAR,"DrawLine");
        DllFunc(hdc,x,y);
      }
      else if(DrawFigures==RectangleDraw)
      {
        coordinates[1].x=x;
        coordinates[1].y=y;
        void (*DllFunc) (HDC hdc,POINT coor[2]);
        DllFunc=(void (*) (HDC hdc,POINT coor[2])) GetProcAddress( DLLGREBIAR,"DrawRectangle");
        DllFunc(hdc,coordinates);
      }
      else if(DrawFigures==CirclDraw)
      {
        coordinates[1].x=x;
        coordinates[1].y=y;
        void (*DllFunc) (HDC hdc,POINT coor[2]);
        DllFunc=(void (*) (HDC hdc,POINT coor[2])) GetProcAddress( DLLGREBIAR,"DrawCircl");
        DllFunc(hdc,coordinates);
      }
      break;
    }
    case WM_COMMAND:
    {
      if(LOWORD(wParam) == button_id1)
      {
        DrawFigures=LineDraw;
      }
      else if(LOWORD(wParam) == button_id2)
      {
        DrawFigures=RectangleDraw;
      }
      else if(LOWORD(wParam) == button_id3)
      {
        DrawFigures=CirclDraw;
      }
      else if(LOWORD(wParam)== button_id7)
      {
        CHAR editText1[1],editText2[1],editText3[1];
        GetWindowText(edit1, editText1 ,4);
        int REDIT=atoi (editText1);
        REDIT%256;
        GetWindowText(edit2, editText2 ,4);
        int GEDIT=atoi (editText2);
        GEDIT%256;
        GetWindowText(edit3, editText3 ,4);
        int BEDIT=atoi (editText3);
        BEDIT%256;
        COLORREF Color = RGB(REDIT,GEDIT,BEDIT);
        void (*DllFunc) (HDC hdc,COLORREF Color);
        DllFunc=(void (*) (HDC hdc,COLORREF Color)) GetProcAddress( DLLGREBIAR,"updateColor");
        DllFunc(hdc,Color);
      }
      break;
    }
    case WM_DESTROY:
    {
      void (*DllFunc) ();
      DllFunc=(void (*) ()) GetProcAddress( DLLGREBIAR,"FreeTool");
      DllFunc();
      FreeLibrary(DLLGREBIAR);
      PostQuitMessage(0);
    }
    default:
      return DefWindowProcA(hwnd, msg, wParam, lParam);
  }
  return DefWindowProcA(hwnd, msg, wParam, lParam);
}