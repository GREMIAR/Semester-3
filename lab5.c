#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct saveCor
{
  int x;
  int y;
  int x1;
  int y1;
};

struct MainPoly
{
  struct saveCor MassCor[100];
  BOOL CLOSE;
  BOOL DRAW;
  int size;
  COLORREF colorIn;
  COLORREF color;
};

struct MainPoly MainMas[100];

HPEN pen;
HBRUSH brush;
int Mainsize;
HWND HWNDMas[14];
void Drawer(HDC hdc,int x,int y,COLORREF GetColor,COLORREF GetColorIn);
void FreeTool();
void DrawLine(HDC hdc,POINT coor[2]);
void updateColor(HDC hdc,COLORREF Color,COLORREF ColorIn);
COLORREF GetColorIn();
COLORREF GetColor();
void RegClass(WNDPROC,LPCTSTR);
BOOL intersection(POINT c[4]);

enum stateDraw{
  One,
  Two,
  Three,
  Four,
  Five,
  Six,
  Seven
};

enum stateDraw Condition;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  HWND hwndMain = CreateWindow("MainWin", "Main Window", WS_OVERLAPPEDWINDOW, 40, 40, 1100, 720, NULL, NULL, NULL, NULL);
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
      HWNDMas[0] = CreateWindow("static", "Color RGB", WS_VISIBLE | WS_CHILD, 80, 15, 75, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[1] = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, 50, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[2] = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, 100, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[3] = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, 150, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[4] = CreateWindow("static", "Color RGB in", WS_VISIBLE | WS_CHILD, 915, 15, 90, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[5] = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, 885, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[6]= CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, 935, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[7]= CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, 985, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[8]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 65, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[9]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 115, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[10]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 165, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[11]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 900, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[12]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 950, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[13]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 1000, 38, 30, 20, hwnd, NULL, NULL, NULL);
      break;
    }
    case WM_LBUTTONDOWN:
    { 
      if (Condition==One)
      {
        for(int i=0;i<14;i++)
        {
            ShowWindow(HWNDMas[i],SW_HIDE);
        }
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x=LOWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y=HIWORD(lParam);
        MainMas[Mainsize].color=GetColor();
      }
      else
      {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].x1;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].y1;
      }
      Condition=Three;
      break;
    }
    case WM_LBUTTONUP:
    {
      Condition=Two;
      POINT c[4];
      c[0].x=-MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x;
      c[0].y=-MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y;
      c[1].x=-MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1;
      c[1].y=-MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y1;
      for(int i=0;i<MainMas[Mainsize].size-1;i++)
      {
        c[2].x=-MainMas[Mainsize].MassCor[i].x;
        c[2].y=-MainMas[Mainsize].MassCor[i].y;
        c[3].x=-MainMas[Mainsize].MassCor[i].x1;
        c[3].y=-MainMas[Mainsize].MassCor[i].y1;
        if(intersection(c)==TRUE)
        {
          MessageBox(hwnd,"You crossed the line","Error", MB_OK|MB_APPLMODAL);
          MainMas[Mainsize].size--;
          MainMas[Mainsize].size++;
          Condition=Three;
          return 1;
        }
      }
      MainMas[Mainsize].size++;
      break;
    }
    case WM_MOUSEMOVE:
    {
      if(Condition==Three)
      {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1=LOWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y1=HIWORD(lParam);
      }
      if(Condition==One)
      {
          RECT rcClientRect;
          GetClientRect(hwnd, &rcClientRect);
          rcClientRect.top=60;
          InvalidateRect(hwnd,&rcClientRect,1);
      }
      else
      {
          InvalidateRect(hwnd,NULL,1);
      }
      
      break;
    }
    case WM_RBUTTONDOWN:
    {
      if (Condition==Two)
      {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].x1;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].y1;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1=MainMas[Mainsize].MassCor[0].x;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y1=MainMas[Mainsize].MassCor[0].y;
        POINT c[4];
        c[0].x=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x;
        c[0].y=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y;
        c[1].x=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1;
        c[1].y=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y1;
        for(int i=1;i<MainMas[Mainsize].size-1;i++)
        {
          c[2].x=MainMas[Mainsize].MassCor[i].x;
          c[2].y=MainMas[Mainsize].MassCor[i].y;
          c[3].x=MainMas[Mainsize].MassCor[i].x1;
          c[3].y=MainMas[Mainsize].MassCor[i].y1;
          if(intersection(c)==TRUE)
          {
            MessageBox(hwnd,"You crossed the line\nCan't finish","Error2", MB_OK|MB_APPLMODAL);
            MainMas[Mainsize].size--;
            MainMas[Mainsize].size++;
            Condition=Three;
            return 1;
          }
        }
        POINT coordinates[2];
        coordinates[0].x=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x;
        coordinates[0].y=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y;
        coordinates[1].x=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1;
        coordinates[1].y=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].y1;
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
        POINT ptPoints[MainMas[Mainsize].size+1];
        for(int i=0;i<MainMas[Mainsize].size+1;i++)
        {
          ptPoints[i].x=MainMas[Mainsize].MassCor[i].x;
          ptPoints[i].y=MainMas[Mainsize].MassCor[i].y;
        }
        DrawLine(hdc,coordinates);
        DeleteObject(hdc);
        Mainsize++;
        Condition=One;
        for(int i=0;i<14;i++)
        {
          ShowWindow(HWNDMas[i],SW_SHOW);
        }
      }
      else if(Condition==One)
      {
        int colper=0;
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        POINT c[4];
        c[0].x=x;
        c[0].y=y;
        c[1].x=2000;
        c[1].y=2000;
        for (int f=0;f<Mainsize;f++)
        {
          for(int i=0;i<MainMas[f].size+1;i++)
          {
            c[2].x=MainMas[f].MassCor[i].x;
            c[2].y=MainMas[f].MassCor[i].y;
            c[3].x=MainMas[f].MassCor[i].x1;
            c[3].y=MainMas[f].MassCor[i].y1;
            if(intersection(c)==TRUE)
            {
              colper+=1;
              Condition=Two;
            }
          }
        }
        if(colper %2 == 1)
        {
          HDC hdc = GetDC (hwnd);
          colper=0;
          int f=0;
          for (f;f<Mainsize;f++)
          {
            for(int i=0;i<MainMas[f].size+1;i++)
            {
              c[2].x=MainMas[f].MassCor[i].x;
              c[2].y=MainMas[f].MassCor[i].y;
              c[3].x=MainMas[f].MassCor[i].x1;
              c[3].y=MainMas[f].MassCor[i].y1;
              if(intersection(c)==TRUE)
              {
                colper+=1;
                Condition=Two;
              }
            }
            if(colper==1&&MainMas[f].DRAW==FALSE)
            {
              MainMas[f].DRAW=TRUE;
              MainMas[f].colorIn=GetColorIn();
              break;
            }
            colper=0;
          }      
          updateColor(hdc,MainMas[f].color,RGB(185,209,234));
          POINT ptPoints[MainMas[f].size+1];
          for(int a=0;a<MainMas[f].size+1;a++)
          {
            ptPoints[a].x=MainMas[f].MassCor[a].x;
            ptPoints[a].y=MainMas[f].MassCor[a].y;
          }
          Polygon(hdc, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
          Drawer(hdc,x,y,MainMas[f].color,MainMas[f].colorIn);
          Condition=One;
          DeleteObject(hdc);
        }
        else
        {
          MessageBox(hwnd,"You didn't get anywhere","Error3", MB_OK|MB_APPLMODAL);
        }
      }
      break;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      if((Condition!=Two))
      {
        HDC hdc = BeginPaint(hwnd,&ps);
        HDC memDC = CreateCompatibleDC(hdc);
        RECT rcClientRect;
        GetClientRect( hwnd, &rcClientRect );
        HBITMAP bmp = CreateCompatibleBitmap( hdc,  rcClientRect.right , rcClientRect.bottom );
        SelectObject(memDC,bmp);
        FillRect(memDC,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 23));
        POINT coordinates[2];
        for(int f=0;f<Mainsize+1;f++)
        {
          for(int i=0;i<MainMas[f].size+1;i++)
          {
            coordinates[0].x=MainMas[f].MassCor[i].x;
            coordinates[0].y=MainMas[f].MassCor[i].y;
            coordinates[1].x=MainMas[f].MassCor[i].x1;
            coordinates[1].y=MainMas[f].MassCor[i].y1;
            updateColor(memDC,MainMas[f].color,MainMas[f].colorIn);
            if (i!=0)
            {
              coordinates[0].x=MainMas[f].MassCor[i-1].x1;
              coordinates[0].y=MainMas[f].MassCor[i-1].y1;
              DrawLine(memDC,coordinates);
            }
            else
            {
              DrawLine(memDC,coordinates);
            }
          }
          if(MainMas[f].DRAW==TRUE)
          {
            POINT ptPoints[MainMas[f].size+1];
            for(int a=0;a<MainMas[f].size+1;a++)
            {
              ptPoints[a].x=MainMas[f].MassCor[a].x;
              ptPoints[a].y=MainMas[f].MassCor[a].y;
            }
            Polygon(memDC, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
          }
        }
        BitBlt(hdc,0,0,rcClientRect.right,rcClientRect.bottom,memDC,0,0,SRCCOPY);
        DeleteObject(bmp);
        DeleteDC(memDC);
        EndPaint(hwnd,&ps);
      }
      break;
    }
    case WM_ERASEBKGND:
    {
      return 1;
    }
    case WM_DESTROY:
    {
      FreeTool();
      PostQuitMessage(0);
    }
    default:
      return DefWindowProcA(hwnd, msg, wParam, lParam);
  }
  return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void DrawLine(HDC hdc,POINT coor[2]) 
{
  MoveToEx(hdc, coor[0].x, coor[0].y, NULL);
  LineTo(hdc, coor[1].x, coor[1].y);
}

BOOL intersection(POINT c[4])
{
  double v1=(c[3].x-c[2].x)*(c[0].y-c[2].y)-(c[3].y-c[2].y)*(c[0].x-c[2].x);
  double v2=(c[3].x-c[2].x)*(c[1].y-c[2].y)-(c[3].y-c[2].y)*(c[1].x-c[2].x);
  double v3=(c[1].x-c[0].x)*(c[2].y-c[0].y)-(c[1].y-c[0].y)*(c[2].x-c[0].x);
  double v4=(c[1].x-c[0].x)*(c[3].y-c[0].y)-(c[1].y-c[0].y)*(c[3].x-c[0].x);
  return ((v1*v2<=0) && (v3*v4<=0));
}

COLORREF GetColorIn()
{
  CHAR editText1[1],editText2[1],editText3[1];
  GetWindowText(HWNDMas[8], editText1 ,4);
  int REDIT=atoi (editText1);
  REDIT%256;
  GetWindowText(HWNDMas[9], editText2 ,4);
  int GEDIT=atoi (editText2);
  GEDIT%256;
  GetWindowText(HWNDMas[10], editText3 ,4);
  int BEDIT=atoi (editText3);
  BEDIT%256;
  return RGB(REDIT,GEDIT,BEDIT);
}

COLORREF GetColor()
{
  CHAR editText1[1],editText2[1],editText3[1];
  GetWindowText(HWNDMas[11], editText1 ,4);
  int REDIT=atoi (editText1);
  REDIT%256;
  GetWindowText(HWNDMas[12], editText2 ,4);
  int GEDIT=atoi (editText2);
  GEDIT%256;
  GetWindowText(HWNDMas[13], editText3 ,4);
  int BEDIT=atoi (editText3);
  BEDIT%256;
  return RGB(REDIT,GEDIT,BEDIT);
}

void FreeTool() 
{
  DeleteObject(pen);
  DeleteObject(brush);
}

void updateColor(HDC hdc,COLORREF Color,COLORREF ColorIn) 
{
  DeleteObject(pen);
  DeleteObject(brush);
  pen = CreatePen(PS_SOLID, 5, Color);
  brush = CreateSolidBrush(ColorIn);
  SelectObject(hdc, pen);
  SelectObject(hdc, brush);
}

void Drawer(HDC hdc,int x,int y,COLORREF GetColor,COLORREF GetColorIn)
{
  COLORREF Background = GetPixel(hdc,x,y);
  if (Background==GetColorIn||Background==GetColor)
  {
    return;
  }
  SetPixel(hdc,x,y,GetColorIn);
  Drawer(hdc,x,y-1,GetColor,GetColorIn);
  Drawer(hdc,x-1,y,GetColor,GetColorIn);
  Drawer(hdc,x,y+1,GetColor,GetColorIn);
  Drawer(hdc,x+1,y,GetColor,GetColorIn);
}