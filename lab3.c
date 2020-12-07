#include <windows.h>
#include <stdio.h>
#include <Richedit.h>

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define NewMDI 1
#define Close 2
#define Exit 3
#define OpenFile 5

void RegClass(WNDPROC,LPCTSTR); 
HWND hwndMain;
HWND hwndClient;
HWND hwndChild;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{  
  RegClass(FrameWndProc,"MainWin");
  hwndMain = CreateWindow("MainWin", "Main Window", WS_OVERLAPPEDWINDOW, 40, 40, 1080, 720, NULL, NULL, NULL, NULL);
  ShowWindow(hwndMain, SW_SHOWNORMAL);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    if(!TranslateMDISysAccel(hwndClient, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
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

BOOL CALLBACK ClientWndProc(HWND hWnd,LPARAM lParam)
{  
  printf("3");  
  HWND hwndEdit = (HWND)GetWindowLongPtr(hWnd, GWLP_USERDATA);
  LPSTR str = (LPSTR)GetWindowLongPtr(hwndClient, GWLP_USERDATA);
  SetWindowText(hwndEdit,str);
  return TRUE;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_CREATE:
    {
      RegClass(ChildWndProc,"ChildWin");
      CLIENTCREATESTRUCT ccs; 
      ccs.hWindowMenu = NULL; 
      ccs.idFirstChild = 100; 
      hwndClient = CreateWindow( "MDICLIENT", (LPCTSTR) NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE, 0, 0, 0, 0, hwnd, NULL, NULL, &ccs); 
      HMENU menu = CreateMenu();
      HMENU hSubMenu = CreatePopupMenu();
      AppendMenu(hSubMenu,MF_STRING,NewMDI,"&Open");
      AppendMenu(hSubMenu,MF_STRING,OpenFile,"&OpenFile");
      AppendMenu(hSubMenu,MF_STRING,Close,"&Close");
      AppendMenu(hSubMenu,MF_STRING,Exit,"&Exit");
      AppendMenu(menu,MF_STRING| MF_POPUP,hSubMenu,"&File");
      SetMenu(hwnd,menu);
      break;
    }
    case WM_COMMAND:
    {
      if(LOWORD(wParam)==NewMDI)
      { 
        CreateMDIWindow("ChildWin","MDIWINDOW",MDIS_ALLCHILDSTYLES,CW_USEDEFAULT,CW_USEDEFAULT,480,200,hwndClient,NULL,NULL);
        SendMessage(hwndClient, WM_MDITILE,NULL,NULL);
      }
      else if(LOWORD(wParam)==Close)
      {
        DestroyWindow(SendMessage(hwndClient,WM_MDIGETACTIVE,NULL,NULL));
      }
      else if(LOWORD(wParam)==Exit)
      {
        PostQuitMessage(0);
      }
      else if(LOWORD(wParam)==OpenFile)
      {
        char buff[1024];
        HANDLE hFile;
        DWORD lpNumberOfBytesRead;
        hFile = CreateFile("text.txt",GENERIC_READ,  0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL); 
        do 
        {
          ReadFile( hFile, buff, sizeof(buff), &lpNumberOfBytesRead, NULL); 
        }
        while(lpNumberOfBytesRead != 0);
        CloseHandle(hFile); 
        SetWindowLongPtr(hwndClient, GWLP_USERDATA,(LONG_PTR)buff);
        EnumChildWindows(hwndClient,EnumFunc,NULL); 
      }
      break;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
    }
    default:
    {
      return DefFrameProc(hwnd,hwndClient, msg, wParam, lParam);
    }
  }
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_CREATE:
    {
      HINSTANCE hRTFLib = LoadLibrary("RICHED32.DLL");
      HWND hwndEdit = CreateWindow("RICHEDIT",NULL,WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL |ES_NOHIDESEL | ES_AUTOVSCROLL | ES_MULTILINE | ES_SAVESEL,0,0,0,0,hwnd, NULL, NULL, NULL);
      SendMessage(hwndEdit,EM_SETEVENTMASK,NULL,ENM_CHANGE);
      SetWindowLongPtr(hwnd, GWLP_USERDATA,(LONG_PTR)hwndEdit);
      break;
    }
    case WM_COMMAND:
    {
      if(HIWORD(wParam) == EN_CHANGE)
			{
        LPSTR textRech = malloc (sizeof(CHAR)*1024);
        HWND hwndEdit = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        SendMessage(hwndEdit,WM_GETTEXT,1024,textRech);
        SetWindowLongPtr(hwndClient, GWLP_USERDATA,(LONG_PTR)textRech);
        EnumChildWindows(hwndClient,EnumFunc,NULL); 
      }
      break;
    }
    case WM_SIZE:
    {
      HWND hwndEdit = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      RECT rc;
      GetClientRect(hwnd, &rc);
      MoveWindow(hwndEdit,0, 0, rc.right - rc.left, rc.bottom - rc.top,1);
      break;
    }
    case WM_SETFOCUS:
    {
      hwndChild=hwnd;
      HWND hwndEdit = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      LPSTR str = (LPSTR)GetWindowLongPtr(hwndClient, GWLP_USERDATA);
      SetWindowText(hwndEdit,str);
      break;
    }
    case WM_CLOSE:
    {
      SendMessage(hwndClient, WM_MDITILE,NULL,NULL);
      DestroyWindow(hwnd);
      break;
    }
    default:
      return DefMDIChildProc(hwnd, msg, wParam, lParam);
  }
}


























































