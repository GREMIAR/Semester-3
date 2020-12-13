#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegClass(WNDPROC,LPCTSTR);

enum which{
  One1,//Главное окно отображено
  One2,
  One3,
  One4,
  Two1,//Добавлен элемент фигуры
  Two2,
  Two3,
  Three1,//Проверка на пересечение сторон(ЛКМ)
  Three2,
  Four
};

struct field{
    BOOL Empty;
    BOOL Alive;
    enum which Who;
    int remained;
};
struct field Our[9][9];
struct field Bot[10][10];

void Buum(HDC hdc,int One,int Two);
void Miss(HDC hdc,int One,int Two);

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
            for(int i=0;i<11;i++)
            {
                for(int f=0;f<11;f++)
                {
                     Bot[i][f].Empty=TRUE;
                     Bot[i][f].Alive =TRUE;
                }
            }
            Bot[0][0].Empty=FALSE;
            Bot[0][0].Who=One1;

            Bot[0][9].Empty=FALSE;
            Bot[0][9].Who=One2;

            Bot[9][0].Empty=FALSE;
            Bot[9][0].Who=One3;

            Bot[9][9].Empty=FALSE;
            Bot[9][9].Who=One4;

            Bot[2][0].Empty=FALSE;
            Bot[2][0].Who=Two1;
            Bot[3][0].Empty=FALSE;
            Bot[3][0].Who=Two1;

            Bot[2][9].Empty=FALSE;
            Bot[2][9].Who=Two2;
            Bot[3][9].Empty=FALSE;
            Bot[3][9].Who=Two2;

            Bot[9][6].Empty=FALSE;
            Bot[9][6].Who=Two3;
            Bot[9][7].Empty=FALSE;
            Bot[9][7].Who=Two3;

            Bot[5][0].Empty=FALSE;
            Bot[5][0].Who=Three1;
            Bot[6][0].Empty=FALSE;
            Bot[6][0].Who=Three1;
            Bot[7][0].Empty=FALSE;
            Bot[7][0].Who=Three1;

            Bot[9][2].Empty=FALSE;
            Bot[9][2].Who=Three2;
            Bot[9][3].Empty=FALSE;
            Bot[9][3].Who=Three2;
            Bot[9][4].Empty=FALSE;
            Bot[9][4].Who=Three2;

            Bot[0][2].Empty=FALSE;
            Bot[0][2].Who=Four;
            Bot[0][3].Empty=FALSE;
            Bot[0][3].Who=Four;
            Bot[0][4].Empty=FALSE;
            Bot[0][4].Who=Four;
            Bot[0][5].Empty=FALSE;
            Bot[0][5].Who=Four;

            for(int i=0;i<11;i++)
            {
                for(int f=0;f<11;f++)
                {
                    if(Bot[i][f].Who==One1) Bot[i][f].remained=1;
                    if(Bot[i][f].Who==One2) Bot[i][f].remained=1;
                    if(Bot[i][f].Who==One3) Bot[i][f].remained=1;
                    if(Bot[i][f].Who==One4) Bot[i][f].remained=1;
                    if(Bot[i][f].Who==Two1) Bot[i][f].remained=2;
                    if(Bot[i][f].Who==Two2) Bot[i][f].remained=2;
                    if(Bot[i][f].Who==Two3) Bot[i][f].remained=2;
                    if(Bot[i][f].Who==Three1) Bot[i][f].remained=3;
                    if(Bot[i][f].Who==Three2) Bot[i][f].remained=3;
                    if(Bot[i][f].Who==Four) Bot[i][f].remained=4;
                    
                }
            }
        
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
            printf("y=%d,x=%d\n",x,y);
            int One,Two;
            if((y-34)>=0&&y<364)
            {
                One=(y-34)/33;
            }
            else
            {
                return;
            }
            if((x-451)>=0&&x<780)
            {
                Two=(x-451)/33;
            }
            else
            {
                return;
            }
            if (Bot[One][Two].Empty==FALSE)
            {
                HDC hdc = GetDC (hwnd);
                if(Bot[(y-34)/33][(x-451)/33].Alive==TRUE)
                {
                    Buum(hdc,One,Two);
                    Bot[One][Two].Alive=FALSE;
                    for(int i=0;i<10;i++)
                    {
                        for(int f=0;f<10;f++)
                        {
                            if(!Bot[i][f].Empty&&Bot[i][f].Who==Bot[One][Two].Who)
                            {
                                if(Bot[i][f].remained>0)
                                {
                                    Bot[i][f].remained--;
                                }
                            }
                        }
                    }
                }
                for(int i=0;i<10;i++)
                {
                    for(int f=0;f<10;f++)
                    {
                        if(!Bot[i][f].Empty&&Bot[i][f].Who==Bot[One][Two].Who&&Bot[i][f].remained==0)
                        {
                            if(Bot[i-1][f-1].Empty&&(i-1>=0)&&(f-1>=0))Miss(hdc,i-1,f-1);
                            if(Bot[i][f-1].Empty&&(f-1>=0))Miss(hdc,i,f-1);
                            if(Bot[i-1][f].Empty&&(i-1>=0))Miss(hdc,i-1,f);
                            if(Bot[i+1][f].Empty&&(i+1<=9))Miss(hdc,i+1,f);
                            if(Bot[i][f+1].Empty&&(f+1<=9))Miss(hdc,i,f+1);
                            if(Bot[i-1][f+1].Empty&&(i-1>=0)&&(f+1<=9))Miss(hdc,i-1,f+1);
                            if(Bot[i+1][f+1].Empty&&(i+1<=9)&&(f+1<=9))Miss(hdc,i+1,f+1);
                            if(Bot[i+1][f-1].Empty&&(i+1<=9)&&(f-1>=0))Miss(hdc,i+1,f-1);
                        }
                    }
                }
            }
            else
            {
                HDC hdc = GetDC (hwnd);
                Miss(hdc,One,Two);
            }
            
           /* for(int i=0;i<10;i++)
            {
                for(int f=0;f<10;f++)
                {
                    if(Bot[i][f].Empty==FALSE)
                    {
                        //printf("y=%d,x=%d\n",f,i);
                    }
                }
            }*/

            
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


void Buum(HDC hdc,int One,int Two)
{
    One=35+One*33;
    Two=450+Two*33;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
    HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Rectangle(hdc,Two+4, One+3, Two+33-4,One+33-6);
}

void Miss(HDC hdc,int One,int Two)
{
    One=35+One*33;
    Two=450+Two*33;
    
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(255,0,0));
    HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Ellipse(hdc,Two+14, One+13, Two+33-14,One+33-16);
}