#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>
#pragma comment(lib,"ComCtl32.Lib")

#define St 1
#define Bt1 2
#define Bt2 3
HBITMAP bitMap;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


enum Cond
{
  Start, On_On, Off_On, Off_Off, On_Off, Exit
}Condition_Gif_Text;
enum Button_P
{
  Gifka, Textiq, Quit
}Button_Pressed;


void ChangeThreads(int i);
struct Threads
{
  HANDLE Thread;
  HWND hwndB;
};
struct Threads ThreadMas[2];
void RegClass(WNDPROC,LPCTSTR);
DWORD WINAPI LineThread(LPVOID data);
DWORD WINAPI AnimationThread(LPVOID data);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  HWND hwnd = CreateWindow("MainWin", "Thread", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 500, 75, 870, 650, NULL, NULL, NULL, NULL);
  ShowWindow(hwnd, SW_SHOWNORMAL);
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
          Condition_Gif_Text=Start;
          Transition();

          ThreadMas[0].hwndB = CreateWindow("button", "Stop the running line", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,20, 535, 160, 30, hwnd, (HMENU)Bt1, NULL, NULL);
          ThreadMas[1].hwndB = CreateWindow("button", "Stop the animation", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,690, 535, 160, 30, hwnd, (HMENU)Bt2, NULL, NULL);
          HWND hStatus=CreateStatusWindow(WS_CHILD | WS_VISIBLE, "HAPPY NEW YEAR" ,hwnd, St);
          bitMap = NULL;
          break;
        }
        case WM_COMMAND:
        {
          if(LOWORD(wParam)==Bt1)
          { 
            Button_Pressed=Textiq;
            Transition();
            if(Condition_Gif_Text==On_On||Condition_Gif_Text==Off_On)
            {
              SetWindowText(ThreadMas[0].hwndB,"Stop the running line");
            }
            else if (Condition_Gif_Text==On_Off||Condition_Gif_Text==Off_Off)
            {
              SetWindowText(ThreadMas[0].hwndB,"Play the running line");
            }
          }
          else if (LOWORD(wParam)==Bt2)
          {
            Button_Pressed=Gifka;
            Transition();
            if(Condition_Gif_Text==On_Off||Condition_Gif_Text==On_On)
            {
              SetWindowText(ThreadMas[1].hwndB,"Stop the animation");
            }
            else if (Condition_Gif_Text==Off_On||Condition_Gif_Text==Off_Off)
            {
              SetWindowText(ThreadMas[1].hwndB,"Play the animation");
            }
          }
          break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HDC memDC;
            BITMAP bm;
            memDC = CreateCompatibleDC(hdc);
            if(bitMap != NULL)
            {
              GetObject(bitMap, sizeof(bm), &bm);
              SelectObject(memDC, bitMap);
              BitBlt(hdc, 200, 200, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
            }
            DeleteDC(memDC);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
        {
          if (Condition_Gif_Text==Exit)
          {
          CloseHandle(ThreadMas[0].Thread);
          CloseHandle(ThreadMas[1].Thread);
          PostQuitMessage(0);
          }
        }
        default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void Transition()
{
  if (Condition_Gif_Text==Start)
  {
    ThreadMas[0].Thread = CreateThread(NULL, 0, AnimationThread, NULL, 0, NULL);
    ThreadMas[1].Thread = CreateThread(NULL, 0, LineThread, NULL, 0, NULL);
    Condition_Gif_Text=On_On;
  }
  else if (Condition_Gif_Text==On_On&&Button_Pressed==Textiq)
  {
    //ResumeThread(ThreadMas[0].Thread);
    SuspendThread(ThreadMas[1].Thread);
    Condition_Gif_Text=On_Off;
  }
  else if (Condition_Gif_Text==On_On&&Button_Pressed==Gifka)
  {
    SuspendThread(ThreadMas[0].Thread);
    //ResumeThread(ThreadMas[1].Thread);
    Condition_Gif_Text=Off_On;
  }
  else if (Condition_Gif_Text==On_On&&Button_Pressed==Quit)
  {
    CloseHandle(ThreadMas[0].Thread);
    CloseHandle(ThreadMas[1].Thread);
    Condition_Gif_Text=Exit;
  }
  else if (Condition_Gif_Text==Off_On&&Button_Pressed==Textiq)
  {
    //SuspendThread(ThreadMas[0].Thread);
    SuspendThread(ThreadMas[1].Thread);
    Condition_Gif_Text=Off_Off;
  }
  else if (Condition_Gif_Text==Off_On&&Button_Pressed==Gifka)
  {
    ResumeThread(ThreadMas[0].Thread);
    //ResumeThread(ThreadMas[1].Thread);
    Condition_Gif_Text=On_On;
  }
  else if (Condition_Gif_Text==Off_On&&Button_Pressed==Quit)
  {
    CloseHandle(ThreadMas[0].Thread);
    CloseHandle(ThreadMas[1].Thread);
    Condition_Gif_Text=Exit;
  }
  else if (Condition_Gif_Text==Off_Off&&Button_Pressed==Textiq)
  {
    //SuspendThread(ThreadMas[0].Thread);
    ResumeThread(ThreadMas[1].Thread);
    Condition_Gif_Text=Off_On;
  }
  else if (Condition_Gif_Text==Off_Off&&Button_Pressed==Gifka)
  {
    ResumeThread(ThreadMas[0].Thread);
    //SuspendThread(ThreadMas[1].Thread);
    Condition_Gif_Text=On_Off;
  }
  else if (Condition_Gif_Text==Off_Off&&Button_Pressed==Quit)
  {
    CloseHandle(ThreadMas[0].Thread);
    CloseHandle(ThreadMas[1].Thread);
    Condition_Gif_Text=Exit;
  }
  else if (Condition_Gif_Text==On_Off&&Button_Pressed==Textiq)
  {
    //ResumeThread(ThreadMas[0].Thread);
    ResumeThread(ThreadMas[1].Thread);
    Condition_Gif_Text=On_On;
  }
  else if (Condition_Gif_Text==On_Off&&Button_Pressed==Gifka)
  {
    SuspendThread(ThreadMas[0].Thread);
    //SuspendThread(ThreadMas[1].Thread);
    Condition_Gif_Text=Off_Off;
  }
  else if (Condition_Gif_Text==On_Off&&Button_Pressed==Quit)
  {
    CloseHandle(ThreadMas[0].Thread);
    CloseHandle(ThreadMas[1].Thread);
    Condition_Gif_Text=Exit;
  }
  printf("\nCondition changed to %d\n", Condition_Gif_Text);
}

DWORD WINAPI LineThread(LPVOID data){
   while(TRUE){
     Sleep(300);
     printf("T");
   }
}

DWORD WINAPI AnimationThread(LPVOID data){
   while(TRUE){
     Sleep(300);
     printf("G");
   }
}