#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct saveCor
{
  POINT xy;
  POINT x1y1;
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
void Drawer(HDC hdc,COLORREF GetColor,HWND hwnd, int f);
void FreeTool();
void DrawLine(HDC hdc,POINT coor[2]);
void updateColor(HDC hdc,COLORREF Color,COLORREF ColorIn);
COLORREF GetColorIn();
COLORREF GetColor();
void RegClass(WNDPROC,LPCTSTR);
BOOL intersection(POINT c[4]);

enum stateDraw{
  One,//Главное окно отображено
  Two,//Добавлен элемент фигуры
  Three,//Проверка на пересечение сторон(ЛКМ)
  Four,//Отображено окно с ошибкой
  Five,//Проверка на пересечение сторон(ПКМ)
  Six,//Проверка курсор внутри или снаружи
  Seven//Отображено окно с ошибкой
};

enum stateDraw Condition;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  HWND hwndMain = CreateWindow("MainWin", "Paint", WS_OVERLAPPEDWINDOW, 40, 40, 1100, 720, NULL, NULL, NULL, NULL);
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
      HWNDMas[6] = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, 935, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[7] = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, 985, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[8] = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 65, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[9] = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 115, 38, 30, 20, hwnd, NULL, NULL, NULL);
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
        UpdateWindow(hwnd);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x=LOWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y=HIWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x=0;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y=0;
        MainMas[Mainsize].color=GetColor();
        Condition=Two;
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
        MoveToEx(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y, NULL);
        LineTo(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y);
        DeleteObject(hdc);
      }
      else if(Condition==Two)
      {
        if(MainMas[Mainsize].size-1>=0)
        {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].x1y1;
        Condition=Three;
        }
        Condition=Three;
      }
      break;
    }
    case WM_LBUTTONUP:
    {
      if (Condition==Three)
      {
        Condition=Two;
        POINT c[4];
        c[0]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x=LOWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y=HIWORD(lParam);
        c[1]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1;
        for(int i=0;i<MainMas[Mainsize].size-1;i++)
        {
          c[2]=MainMas[Mainsize].MassCor[i].xy;
          c[3]=MainMas[Mainsize].MassCor[i].x1y1;
          if(intersection(c)==TRUE)
          {
            MessageBox(hwnd,"You crossed the line","Error", MB_OK|MB_APPLMODAL);
            MainMas[Mainsize].size--;
            MainMas[Mainsize].size++;
            InvalidateRect(hwnd,NULL,1);
            return 1;
          }
        }
        if(MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x==MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x||MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y==MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y)
        {
            MessageBox(hwnd,"You crossed the line","Error", MB_OK|MB_APPLMODAL);
            MainMas[Mainsize].size--;
            MainMas[Mainsize].size++;
            InvalidateRect(hwnd,NULL,1);
            return 1;
        }
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
        MoveToEx(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y, NULL);
        LineTo(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y);
        MainMas[Mainsize].size++;
        DeleteObject(hdc);
      }
      break;
    }
    case WM_MOUSEMOVE:
    {
      printf("%d",Condition+1);
      if(Condition==One)
      {
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        rcClientRect.top=60;
        InvalidateRect(hwnd,&rcClientRect,1);
      }
      else
      {
        if(Condition==Three)
        {
          MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x=LOWORD(lParam);
          MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y=HIWORD(lParam);
        }
        InvalidateRect(hwnd,NULL,1);
      }
      
      break;
    }
    case WM_RBUTTONDOWN:
    {
      if (Condition==Two)
      {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].x1y1;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1=MainMas[Mainsize].MassCor[0].xy;
        POINT c[4];
        c[0]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy;
        c[1]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1;
        for(int i=1;i<MainMas[Mainsize].size-1;i++)
        {
          c[2]=MainMas[Mainsize].MassCor[i].xy;
          c[3]=MainMas[Mainsize].MassCor[i].x1y1;
          if(intersection(c)==TRUE)
          {
            MessageBox(hwnd,"You crossed the line\nCan't finish","Error", MB_OK|MB_APPLMODAL);
            MainMas[Mainsize].size--;
            MainMas[Mainsize].size++;
            Condition=Two;
            return 1;
          }
        }
        POINT coordinates[2];
        coordinates[0]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy;
        coordinates[1]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1;
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
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
        POINT c[4];
        c[0].x=LOWORD(lParam);
        c[0].y=HIWORD(lParam);
        c[1].x=2000;
        c[1].y=2000;
        for (int f=0;f<Mainsize;f++)
        {
          for(int i=0;i<MainMas[f].size+1;i++)
          {
            c[2]=MainMas[f].MassCor[i].xy;
            c[3]=MainMas[f].MassCor[i].x1y1;
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
              c[2]=MainMas[f].MassCor[i].xy;
              c[3]=MainMas[f].MassCor[i].x1y1;
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
            ptPoints[a]=MainMas[f].MassCor[a].xy;
          }
          Polygon(hdc, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
          Drawer(hdc,MainMas[f].colorIn, hwnd,f);
          Condition=One;
          DeleteObject(hdc);
        }
        else
        {
          MessageBox(hwnd,"You didn't get anywhere","Error", MB_OK|MB_APPLMODAL);
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
            coordinates[0]=MainMas[f].MassCor[i].xy;
            coordinates[1]=MainMas[f].MassCor[i].x1y1;
            updateColor(memDC,MainMas[f].color,MainMas[f].colorIn);
            if (i!=0)
            {
              coordinates[0]=MainMas[f].MassCor[i-1].x1y1;
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
              ptPoints[a]=MainMas[f].MassCor[a].xy;
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

void Drawer(HDC hdc,COLORREF GetColor,HWND hwnd, int f)
{
  int left,right,top,bot;
  left = MainMas[f].MassCor[0].xy.x;
  right = MainMas[f].MassCor[0].xy.x;
  top = MainMas[f].MassCor[0].xy.y;
  bot = MainMas[f].MassCor[0].xy.y;
  for (int i=0; i <= MainMas[f].size; i++)
  {
    if (MainMas[f].MassCor[i].xy.x < left) left = MainMas[f].MassCor[i].xy.x;
    else if (MainMas[f].MassCor[i].xy.x > right) right = MainMas[f].MassCor[i].xy.x;
    if (MainMas[f].MassCor[i].xy.y < top) top = MainMas[f].MassCor[i].xy.y;
    else if (MainMas[f].MassCor[i].xy.y > bot) bot = MainMas[f].MassCor[i].xy.y;
  }
  RECT rcClientRect;
  GetClientRect(hwnd, &rcClientRect);
  POINT c[4];
  c[0].x=left;
  c[0].y=top;
  c[1].x=rcClientRect.right+1;
  c[1].y=rcClientRect.bottom+1;
  for (int i1 = left; i1 < right; i1++)
  {
    for (int j1 = top; j1 < bot; j1++)
    {
      int colper=0;
      c[0].x=i1;
      c[0].y=j1;
      for(int i=0;i<MainMas[f].size+1;i++)
      {
        c[2]=MainMas[f].MassCor[i].xy;
        c[3]=MainMas[f].MassCor[i].x1y1;
        if(intersection(c)==TRUE)
        {
          colper+=1;
        }
      }
      if (colper%2!=0) 
      {
        SetPixel(hdc, i1, j1, GetColor);
      }
    }
  }
}