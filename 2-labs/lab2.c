#include <windows.h>
#include <WinError.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define edit_id 1
#define button_id 2
#define list_id 3

HWND hListBox;
TCHAR editText[1];
HWND edit;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSA wcl;
    memset(&wcl, 0, sizeof(WNDCLASS));
    wcl.lpszClassName = "MainWin";
    wcl.lpfnWndProc = WndProc;
    wcl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 23);
    wcl.hIcon = LoadIcon(NULL, IDI_ERROR);
    wcl.hCursor = LoadCursor(NULL,IDC_ARROW);
    RegisterClassA(&wcl);

    HWND hwnd = CreateWindow("MainWin", "Error handler", WS_CAPTION|WS_SYSMENU , 680, 400, 595, 205, NULL, NULL, NULL, NULL);//680, 400, 335, 272
    HWND textMain = CreateWindow("static", "Error number:", WS_VISIBLE | WS_CHILD  , 20, 20, 90, 20, hwnd, NULL, NULL, NULL);
    edit = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 115, 20, 90, 20, hwnd, (HMENU)edit_id, NULL, NULL);
    HWND button = CreateWindow("button", "Search", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 210, 20, 90, 20, hwnd, (HMENU)button_id, NULL, NULL);
    hListBox = CreateWindow("listbox", NULL,WS_CHILD | WS_VISIBLE | LBS_STANDARD ,20, 60, 550, 100,hwnd, (HMENU) list_id, NULL, NULL);//25, 60, 270, 100
    HWND textMain1 = CreateWindow("static", "1.Error registering the window class.\n2.Error creating the app window.\n3.My Error.", WS_VISIBLE | WS_CHILD, 310, 5, 260, 50, hwnd, NULL, NULL, NULL);
    SendMessage(edit, EM_LIMITTEXT, 1, 0);
    ShowWindow(hwnd, SW_SHOWNORMAL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
    {
        if(LOWORD(wParam) == button_id)
        {
            GetWindowText(edit, editText ,2);
            if(editText[0]=='1')
            {
                TCHAR strmain[100];
                WNDCLASSA wcl;
                memset(&wcl, 0, sizeof(WNDCLASS));
                wcl.lpszClassName = "MainWin";
                RegisterClassA(&wcl);
                DWORD dwError = GetLastError();
                LPTSTR hlocal ;         
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, dwError, MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),&hlocal, 0, NULL);
                sprintf(strmain, "%d ", dwError);
                strncat(strmain,hlocal,100);
                strncat(strmain, " ", 1);
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, dwError, MAKELANGID(LANG_RUSSIAN,SUBLANG_RUSSIAN_RUSSIA),&hlocal, 0, NULL);
                strncat(strmain,hlocal,100);
                SendMessage(hListBox, LB_ADDSTRING, 0,(LPARAM)strmain);
            }
            else if(editText[0]=='2')
            {
                TCHAR strmain[100];
                HWND forerror = CreateWindow("pupum", "handler", WS_OVERLAPPEDWINDOW , 680, 400, 335, 272, NULL, NULL, NULL, NULL);
                DWORD dwError = GetLastError();
                LPTSTR hlocal ;       
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, dwError, MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),&hlocal, 0, NULL);
                sprintf(strmain, "%d ", dwError);
                strncat(strmain,hlocal,100);
                strncat(strmain, " ", 1);
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, dwError, MAKELANGID(LANG_RUSSIAN,SUBLANG_RUSSIAN_RUSSIA),&hlocal, 0, NULL);
                strncat(strmain,hlocal,100);
                SendMessage(hListBox, LB_ADDSTRING, 0,(LPARAM)strmain);
            }
            else if(editText[0]=='3')
            {
                TCHAR strmain[100];
                FILE *fp;
                char name[] = "my.txt";
                fp = fopen(name, "r");
                DWORD dwError = GetLastError();
                LPTSTR hlocal ;       
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, dwError, MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),&hlocal, 0, NULL);
                sprintf(strmain, "%d ", dwError);
                strncat(strmain,hlocal,100);
                strncat(strmain, " ", 1);
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, dwError, MAKELANGID(LANG_RUSSIAN,SUBLANG_RUSSIAN_RUSSIA),&hlocal, 0, NULL);
                strncat(strmain,hlocal,100);
                SendMessage(hListBox, LB_ADDSTRING, 0,(LPARAM)strmain);
            }
            else
            {
                MessageBox(NULL, "You entered the wrong command", "Not found", MB_OK);
            }
        }
    }
    default:
    {
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    }
}

