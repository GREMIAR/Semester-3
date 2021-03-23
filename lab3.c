#include <windows.h>
#include <Richedit.h>

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ClientWndProc(HWND hwnd,LPARAM lParam);

#define NewMDI 1
#define Close 2
#define Exit 3
#define OpenFile 4

void RegClass(WNDPROC,LPCTSTR); 
HWND hwndClient;
HWND hwndChild;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{  
  RegClass(FrameWndProc,"MainWin");
  HWND hwndMain = CreateWindow("MainWin", "Main Window", WS_OVERLAPPEDWINDOW, 40, 40, 1080, 720, NULL, NULL, NULL, NULL);
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
        char buff[100];
        DWORD lpNumberOfBytesRead;
        HANDLE hFile = CreateFile("text.txt",GENERIC_READ,  0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL); 
        do 
        {
          ReadFile( hFile, buff, sizeof(buff), &lpNumberOfBytesRead, NULL); 
        }
        while(lpNumberOfBytesRead != 0);
        CloseHandle(hFile); 
        HWND hwndEdit = (HWND)GetWindowLongPtr(hwndChild, GWLP_USERDATA);
        SetWindowText(hwndEdit,buff);
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

BOOL CALLBACK ClientWndProc(HWND hwnd,LPARAM lParam)
{
  if(FindWindowEx(hwnd,NULL,NULL,NULL)==0)
  {
    HWND hwndEdit = (HWND)GetWindowLongPtr(hwndChild, GWLP_USERDATA);
    if(hwndEdit==hwnd)
    {
      return TRUE;
    }
    SetWindowText(hwnd,lParam);
  }
  return TRUE;
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_CREATE:
    {
      HINSTANCE hRTFLib = LoadLibrary("riched32.dll"); 
      HWND hwndEdit = CreateWindow(RICHEDIT_CLASS,NULL,WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOVSCROLL | ES_AUTOVSCROLL| ES_MULTILINE ,0,0,0,0,hwnd, NULL, NULL, NULL);
      SendMessage(hwndEdit,EM_SETEVENTMASK,NULL,ENM_CHANGE);
      SetWindowLongPtr(hwnd, GWLP_USERDATA,(LONG_PTR)hwndEdit);
      LPSTR textRech=(LPSTR)GetWindowLongPtr(hwndClient, GWLP_USERDATA);
      SetWindowText(hwndEdit,textRech);
      break;
    }
    case WM_COMMAND:
    {
      if(HIWORD(wParam) == EN_CHANGE)
      {
        if(hwndChild==hwnd)
        {
          LPSTR textRech = malloc (sizeof(CHAR)*1024);
          HWND hwndEdit = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
          SendMessage(hwndEdit,WM_GETTEXT,1024,textRech);
          SetWindowLongPtr(hwndClient, GWLP_USERDATA,(LONG_PTR)textRech);
          EnumChildWindows(hwndClient,ClientWndProc,textRech); 
        }
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
      break;
    }
    case WM_CLOSE:
    {
      SendMessage(hwndClient, WM_MDITILE,NULL,NULL);
      DestroyWindow(hwnd);
      if (FindWindowEx(hwndClient,NULL,NULL,NULL)==0)
      {
        SetWindowLongPtr(hwndClient, GWLP_USERDATA,(LONG_PTR)"");
      }
      SendMessage(hwndClient, WM_MDITILE,NULL,NULL);
      break;
    }
    default:
      return DefMDIChildProc(hwnd, msg, wParam, lParam);
  }
}
