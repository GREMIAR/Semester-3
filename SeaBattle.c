#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>
#pragma comment(lib,"ComCtl32.Lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define Bt1 1
#define Bt2 2
#define Bt3 3
#define Bt4 4
#define Bt5 5
#define St 6

HANDLE threads;

enum which{
  One1,
  One2,
  One3,
  One4,
  Two1,
  Two2,
  Two3,
  Three1,
  Three2,
  Four
};
struct field
{
    BOOL Empty;
    BOOL Alive;
    enum which Who;
    int remained;
};
struct field Our[10][10];
struct field Bot[10][10];
BOOL WhoseTurn=TRUE;
BOOL StageGame=TRUE;
int NumberShips=20;
int NumberShipsPlayer=0;
HWND del;
HWND confirm1;
BOOL ConfirmShip=FALSE;
HWND FirstThread;
HWND SecondThread;
void RegClass(WNDPROC,LPCTSTR);
void Buum(HDC hdc,int One,int Two);
void Miss(HDC hdc,int One,int Two);
void Wound(HDC hdc,int One,int Two);
void DrawShip(HDC hdc,int One,int Two);
void Starting(HWND hwndmainw);
void Finished();
BOOL cellAvailable(struct field field[10][10], int one, int two,HWND hwndmainw);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  HWND hwnd = CreateWindow("MainWin", "SeaBattle", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 500, 75, 870, 650, NULL, NULL, NULL, NULL);
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
            for(int i=0;i<11;i++)
            {
                for(int f=0;f<11;f++)
                {
                    Our[i][f].Empty=TRUE;
                    Our[i][f].Alive=TRUE;
                }
            }
            CreateWindow("button", "Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,375, 550, 120, 30, hwnd, (HMENU)Bt1, NULL, NULL);
            del=CreateWindow("button", "Del", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,250, 390, 120, 30, hwnd, (HMENU)Bt2, NULL, NULL);
            confirm1=CreateWindow("button", "Confirm", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,120, 390, 120, 30, hwnd, (HMENU)Bt3, NULL, NULL);
            CreateWindow("static", "   A      B      C      D      E      F      G      H      I      J   ", WS_VISIBLE | WS_CHILD| WS_BORDER, 80, 30, 330, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", NULL, WS_VISIBLE | WS_CHILD| WS_BORDER, 61, 49, 20, 330, hwnd, NULL, NULL, NULL);
            CreateWindow("static", NULL, WS_VISIBLE | WS_CHILD| WS_BORDER, 431, 49, 20, 330, hwnd, NULL, NULL, NULL);
            for(int i=56;i<345;i+=33)
            {
                char buffer [0];
                itoa (i/33,buffer,10);
                CreateWindow("static", buffer, WS_VISIBLE | WS_CHILD, 67, i, 10, 20, hwnd, NULL, NULL, NULL);
                CreateWindow("static", buffer, WS_VISIBLE | WS_CHILD, 437, i, 10, 20, hwnd, NULL, NULL, NULL);
            }
            CreateWindow("static", "10", WS_VISIBLE | WS_CHILD, 63, 353, 15, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "10", WS_VISIBLE | WS_CHILD, 433, 353, 15, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "   A      B      C      D      E      F      G      H      I      J   ", WS_VISIBLE | WS_CHILD| WS_BORDER, 450, 30, 330, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "You", WS_VISIBLE | WS_CHILD| WS_BORDER, 230, 5, 28, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("static", "BOT - Optimized queue", WS_VISIBLE | WS_CHILD| WS_BORDER, 540, 5, 155, 20, hwnd, NULL, NULL, NULL);
            FirstThread = CreateWindow("button", "Play the animation", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,20, 535, 160, 30, hwnd, (HMENU)Bt4, NULL, NULL);
            SecondThread = CreateWindow("button", "Play a running line", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,690, 535, 160, 30, hwnd, (HMENU)Bt5, NULL, NULL);
            HWND hStatus=CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL ,hwnd, St);
            SetWindowLongPtr(hwnd, GWLP_USERDATA,(LONG_PTR)hStatus);
            break;
        }
        case WM_COMMAND:
        {
            if(LOWORD(wParam)==Bt1)
            { 
                if(ConfirmShip)
                {
                    Starting(hwnd);
                    RECT rcClientRect;
                    GetClientRect(hwnd, &rcClientRect);
                    rcClientRect.left=420;
                    InvalidateRect(hwnd,&rcClientRect,1);
                    StageGame=FALSE;
                }
                else
                {
                    HWND MainTextInfo = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    SetWindowText(MainTextInfo,"Please confirm the ships");
                }
            }
            else if (LOWORD(wParam)==Bt2)
            {
                RECT rcClientRect;
                GetClientRect(hwnd, &rcClientRect);
                rcClientRect.right=420;
                InvalidateRect(hwnd,&rcClientRect,1);
                for(int i=0;i<11;i++)
                {
                    for(int f=0;f<11;f++)
                    {
                        Our[i][f].Empty=TRUE;
                        Our[i][f].Alive=TRUE;
                    }
                }
                NumberShipsPlayer=0;
            }
            else if (LOWORD(wParam)==Bt3)
            {
                if(NumberShipsPlayer==20)
                {
                    ConfirmShip=TRUE;
                    ShowWindow(confirm1,SW_HIDE);
                    ShowWindow(del,SW_HIDE);
                }
                else
                {
                    HWND MainTextInfo = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    SetWindowText(MainTextInfo,"Confirm put in more ships");
                }
            }
            else if (LOWORD(wParam)==Bt4)
            {
                /*TCHAR textBut[1024];
                SendMessage(FirstThread,WM_GETTEXT,1024,textBut);
                if(textBut=="Play the animation")
                {
                    SetWindowText(FirstThread,"Stop the animation");
                }
                else
                {
                    SetWindowText(FirstThread,textBut);
                }*/
               // HANDLE thread = CreateThread(NULL,0,thread1,NULL, 0, NULL);
            }
            else if (LOWORD(wParam)==Bt5)
            {
                //SetWindowText(SecondThread,"Stop a running line");
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            if (!StageGame)
            {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                int One,Two;
                if((y-49)>=0&&y<379&&(x-451)>=0&&x<780)
                {
                    One=(y-49)/33;
                    Two=(x-451)/33;
                }
                else
                {
                    return 1;
                }
                WhoseTurn=FALSE;
                if (Bot[One][Two].Empty==FALSE&&Bot[One][Two].Alive==TRUE)
                {
                    HDC hdc = GetDC (hwnd);
                    if(Bot[One][Two].Alive==TRUE)
                    {
                        Wound(hdc,50+One*33,450+Two*33);
                        Bot[One][Two].Alive=FALSE;
                        for(int i=0;i<10;i++)
                        {
                            for(int f=0;f<10;f++)
                            {
                                if(!Bot[i][f].Empty&&Bot[i][f].Who==Bot[One][Two].Who&&Bot[i][f].remained>0)
                                {
                                    Bot[i][f].remained--;
                                    WhoseTurn=TRUE;
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
                                int test1=50+i*33;
                                int test2=450+f*33;
                                if(Bot[i-1][f-1].Empty&&(i-1>=0)&&(f-1>=0))
                                {
                                    Miss(hdc,test1-33,test2-33);
                                    Bot[i-1][f-1].Alive=FALSE;
                                }
                                if(Bot[i][f-1].Empty&&(f-1>=0))
                                {
                                    Miss(hdc,test1,test2-33);
                                    Bot[i][f-1].Alive=FALSE;
                                }
                                if(Bot[i-1][f].Empty&&(i-1>=0))
                                {
                                    Miss(hdc,test1-33,test2);
                                    Bot[i-1][f].Alive=FALSE;
                                }
                                if(Bot[i+1][f].Empty&&(i+1<=9))
                                {
                                    Miss(hdc,test1+33,test2);
                                    Bot[i+1][f].Alive=FALSE;
                                }
                                if(Bot[i][f+1].Empty&&(f+1<=9))
                                {
                                    Miss(hdc,test1,test2+33);
                                    Bot[i][f+1].Alive=FALSE;
                                }
                                if(Bot[i-1][f+1].Empty&&(i-1>=0)&&(f+1<=9))
                                {
                                    Miss(hdc,test1-33,test2+33);
                                    Bot[i-1][f+1].Alive=FALSE;
                                }
                                if(Bot[i+1][f+1].Empty&&(i+1<=9)&&(f+1<=9))
                                {
                                    Miss(hdc,test1+33,test2+33);
                                    Bot[i+1][f+1].Alive=FALSE;
                                }
                                if(Bot[i+1][f-1].Empty&&(i+1<=9)&&(f-1>=0))
                                {
                                    Miss(hdc,test1+33,test2-33);
                                    Bot[i+1][f-1].Alive=FALSE;
                                }
                                Buum(hdc,test1,test2);
                                WhoseTurn=TRUE;
                                NumberShips--;
                            }
                        }
                    }
                    DeleteObject(hdc);
                }
                else if (Bot[One][Two].Empty==TRUE&&Bot[One][Two].Alive==TRUE)
                {
                    HDC hdc = GetDC (hwnd);
                    Bot[One][Two].Alive=FALSE;
                    Miss(hdc,50+One*33,450+Two*33);
                    DeleteObject(hdc);
                }  
                else
                {
                    HWND MainTextInfo = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    SetWindowText(MainTextInfo,"You've been here before");
                    WhoseTurn=TRUE;
                }
                if (NumberShips==0)
                {
                    HWND MainTextInfo = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    SetWindowText(MainTextInfo,"Victory");
                    StageGame=TRUE;
                    Finished();
                    return 1;
                }
                if (!WhoseTurn) 
                {
                    HDC hdc = GetDC (hwnd);
                    int y = rand() % (10);
                    int x = rand() % (10);
                    BOOL DoblMove=FALSE;
                    while(!DoblMove)
                    {
                        while(Our[y][x].Alive==FALSE)
                        {
                            y = rand() % (10);
                            x = rand() % (10);
                        }
                        if(Our[y][x].Empty==FALSE)
                        {
                            Buum(hdc,50+y*33,80+x*33);
                            NumberShipsPlayer--;
                            Our[y][x].Alive=FALSE;
                        }
                        else
                        {
                            Miss(hdc,50+y*33,80+x*33);
                            Our[y][x].Alive=FALSE;
                            DoblMove=TRUE;
                        }
                        if(NumberShipsPlayer==0)
                        {
                            HWND MainTextInfo = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                            SetWindowText(MainTextInfo,"Defeat");
                            StageGame=TRUE;
                            Finished();
                            return 1;
                        }
                    }
                    DeleteObject(hdc);
                }     
            }
            else
            {
                if(NumberShipsPlayer<20)
                { 
                    HDC hdc = GetDC (hwnd);
                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    int One,Two;
                    if((y-49)>=0&&y<379&&(x-81)>=0&&x<410)
                    {
                        One=(y-49)/33;
                        Two=(x-81)/33;
                    }
                    else
                    {
                        return 1;
                    }
                    if(Our[One][Two].Empty==TRUE)
                    {
                        if (cellAvailable(Our, One, Two,hwnd))
                        {
                            DrawShip(hdc,50+One*33,80+Two*33);
                            Our[One][Two].Empty=FALSE;
                            NumberShipsPlayer++;
                            DeleteObject(hdc);
                        }
                    }
                    else
                    {
                        HWND MainTextInfo = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                        SetWindowText(MainTextInfo,"Engaged");
                    }
                }
                else
                {
                    HWND MainTextInfo = (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    SetWindowText(MainTextInfo,"You have already delivered all the ships");
                }
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);
            HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
            SelectObject(hdc, pen); 
            for(int i=79;i<433;i+=33)
            {
                MoveToEx(hdc, i, 50, NULL);
                LineTo(hdc, i, 378);
            }
            for(int i=48;i<385;i+=33)
            {
                MoveToEx(hdc, 80, i, NULL);
                LineTo(hdc, 408, i);
            }
            for(int i=449;i<803;i+=33)
            {
                MoveToEx(hdc, i, 50, NULL);
                LineTo(hdc, i, 378);
            }
            for(int i=48;i<385;i+=33)
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
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
    HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc,Two+4, One+3, Two+29,One+27,5,5);
}

void Miss(HDC hdc,int One,int Two)
{
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(255,0,0));
    HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Ellipse(hdc,Two+12, One+11, Two+20,One+18);
}

void Wound(HDC hdc,int One,int Two)
{
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,255,0));
    HBRUSH brush = CreateSolidBrush(RGB(0,255,0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc,Two+4, One+3, Two+29,One+27,5,5);
}
void Starting(HWND hwndmainw)
{
    HWND MainTextInfo = (HWND)GetWindowLongPtr(hwndmainw, GWLP_USERDATA);
    SetWindowText(MainTextInfo,"Starting");
    NumberShips=20;
    StageGame=TRUE;
    for(int i=0;i<11;i++)
    {
        for(int f=0;f<11;f++)
        {
            Bot[i][f].Empty=TRUE;
            Bot[i][f].Alive=TRUE;
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
}

void Finished()
{
    NumberShipsPlayer=0;
    ConfirmShip=FALSE;
    ShowWindow(confirm1,SW_SHOW);
    ShowWindow(del,SW_SHOW);
    for(int i=0;i<11;i++)
    {
        for(int f=0;f<11;f++)
        {
            Our[i][f].Empty=TRUE;
            Our[i][f].Alive=TRUE;
        }
    }
}

void DrawShip(HDC hdc,int One,int Two)
{
    HBRUSH brush = CreateSolidBrush(RGB(0,0,255));
    SelectObject(hdc, brush);
    RoundRect(hdc,Two+4, One+3, Two+29,One+27,5,5);
}

BOOL cellAvailable(struct field field[10][10], int one, int two,HWND hwndmainw)
{
    HWND MainTextInfo = (HWND)GetWindowLongPtr(hwndmainw, GWLP_USERDATA);
    for (int i = 0; i < 10; i++)
    {
        if ((i == one-1) || (i == one+1))
        {
            for (int j = 0; j < 10; j++)
            {
                if ((j == two-1) || (j == two+1))
                {
                    if (!field[i][j].Empty)
                    {
                        SetWindowText(MainTextInfo,"Another ship is too close");
                        return FALSE;
                    }
                }
            }
        }
    }
    // checking ship max size
    int maxLength;
    if (NumberShipsPlayer < 4) maxLength = 4;
    else if (NumberShipsPlayer < 10) maxLength = 3;
    else if (NumberShipsPlayer < 16) maxLength = 2;
    else maxLength = 1;
    int shipSize = 0;
    for (int i = one-1; i >= 0; i--)
    {
        if (field[i][two].Empty) break; else shipSize++;
    }
    for (int i = one+1; i < 10; i++)
    {
        if (field[i][two].Empty) break; else shipSize++;
    }
    for (int i = two-1; i >= 0; i--)
    {
        if (field[one][i].Empty) break; else shipSize++;
    }
    for (int i = two+1; i < 10; i++)
    {
        if (field[one][i].Empty) break; else shipSize++;
    }
    if (shipSize>=maxLength)
    {
        SetWindowText(MainTextInfo,"Another ship is too close");
        return FALSE;
    }
    // only near current ship checking
    //printf("maxl=%d\nshipS=%d\nNumberShipsPlayer=%d\n",maxLength,shipSize,NumberShipsPlayer);
    if ((NumberShipsPlayer!=0)&&(NumberShipsPlayer!=4)&&(NumberShipsPlayer!=7)&&(NumberShipsPlayer!=10)&&(NumberShipsPlayer!=12)&&(NumberShipsPlayer!=14)&&(NumberShipsPlayer<16))
    {
        // if neighbor cell isnt empty
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if ((i == one && (j == two+1 || j == two-1)) || ((j == two && (i == one+1 || i == one-1))))
                {
                    if (!field[i][j].Empty)
                    {
                        // continue building ship
                        return TRUE;
                    }
                }
            }
        }
    }
    else
    {
        return TRUE;
    }
    SetWindowText(MainTextInfo,"Another ship is too close");
    return FALSE;
}