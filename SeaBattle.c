#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegClass(WNDPROC,LPCTSTR);
struct field{
   BOOL Empty;
   BOOL Alive;
};
struct field Our[9][9];
struct field Bot[9][9];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  HWND hwndMain = CreateWindow("MainWin", "SeaBattle", WS_OVERLAPPEDWINDOW, 40, 40, 875, 720, NULL, NULL, NULL, NULL);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            for(int i=0;i<10;i++)
            {
                for(int f=0;f<10;f++)
                {
                     Bot[i][f].Empty=TRUE;
                     Bot[i][f].Alive =TRUE;
                }
            }
            Bot[0][0].Empty=FALSE;
            Bot[0][9].Empty=FALSE;
            Bot[9][0].Empty=FALSE;
            Bot[9][9].Empty=FALSE;
            Bot[2][0].Empty=FALSE;
            Bot[3][0].Empty=FALSE;
            Bot[5][0].Empty=FALSE;
            Bot[6][0].Empty=FALSE;
            Bot[7][0].Empty=FALSE;
            Bot[9][2].Empty=FALSE;
            Bot[9][3].Empty=FALSE;
            Bot[9][4].Empty=FALSE;
            Bot[9][6].Empty=FALSE;
            Bot[9][7].Empty=FALSE;
            Bot[0][2].Empty=FALSE;
            Bot[0][3].Empty=FALSE;
            Bot[0][4].Empty=FALSE;
            Bot[0][5].Empty=FALSE;
            Bot[2][9].Empty=FALSE;
            Bot[3][9].Empty=FALSE;
            CreateWindow("static", "   A      B      C      D      E      F      G      H      I      J   ", WS_VISIBLE | WS_CHILD| WS_BORDER, 80, 15, 330, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", NULL, WS_VISIBLE | WS_CHILD| WS_BORDER, 61, 34, 20, 330, hwnd, NULL, NULL, NULL);
            int f=1;
            for(int i=41;i<330;i+=33)
            {
                char buffer [33];
                itoa (f,buffer,10);
                CreateWindow("static", buffer, WS_VISIBLE | WS_CHILD, 67, i, 10, 20, hwnd, NULL, NULL, NULL);
                f++;
            }
            CreateWindow("static", "10", WS_VISIBLE | WS_CHILD, 63, 338, 15, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("static", "   A      B      C      D      E      F      G      H      I      J   ", WS_VISIBLE | WS_CHILD| WS_BORDER, 450, 15, 330, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", NULL, WS_VISIBLE | WS_CHILD| WS_BORDER, 431, 34, 20, 330, hwnd, NULL, NULL, NULL);
            f=1;
            for(int i=41;i<330;i+=33)
            {
                char buffer [33];
                itoa (f,buffer,10);
                CreateWindow("static", buffer, WS_VISIBLE | WS_CHILD, 437, i, 10, 20, hwnd, NULL, NULL, NULL);
                f++;
            }
            CreateWindow("static", "10", WS_VISIBLE | WS_CHILD, 433, 338, 15, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "You", WS_VISIBLE | WS_CHILD, 230, 400, 30, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "OPTIMIZIROVANAI OCHERED", WS_VISIBLE | WS_CHILD, 520, 400, 200, 20, hwnd, NULL, NULL, NULL);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            printf("x=%d,y=%d\n",(y-34)/33,(x-451)/33);
            if (Bot[(y-34)/33][(x-451)/33].Empty==FALSE)
            {
                printf("Pum\n");
                if(Bot[(y-34)/33][(x-451)/33].Alive==TRUE)
                {
                    printf("1\n");
                    HDC hdc = GetDC (hwnd);

                }
            }
            for(int i=0;i<10;i++)
            {
                for(int f=0;f<10;f++)
                {
                    if(Bot[i][f].Empty==FALSE)
                    {
                        printf("x=%d,y=%d\n",i,f);
                    }
                }
            }

            
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
            HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
            SelectObject(hdc, pen);
            SelectObject(hdc, brush);
            for(int i=79;i<433;i+=33)
            {
                MoveToEx(hdc, i, 35, NULL);
                LineTo(hdc, i, 363);
            }
            for(int i=33;i<370;i+=33)
            {
                MoveToEx(hdc, 80, i, NULL);
                LineTo(hdc, 408, i);
            }

            for(int i=449;i<803;i+=33)
            {
                MoveToEx(hdc, i, 35, NULL);
                LineTo(hdc, i, 363);
            }
            for(int i=33;i<370;i+=33)
            {
                MoveToEx(hdc, 450, i, NULL);
                LineTo(hdc, 778, i);
            }

            EndPaint(hwnd,&ps);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}