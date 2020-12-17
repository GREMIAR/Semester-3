#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



#define button_id1 1
#define button_id2 2
#define list_id 3
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
BOOL DefaultS=FALSE;
BOOL Entry=FALSE;
HWND hwndMain;
int Ind;
struct MainPoly MainMas[100];
struct MainPoly ArchiveMas[100];
HWND ListBox;
HPEN pen;
HBRUSH brush;
int Mainsize;
int Archivesize;
HWND HWNDMas[16];
void Drawer(HDC hdc,COLORREF GetColor, int f);
void Drawer2(HDC hdc, COLORREF GetColor, struct MainPoly tmp);
void FreeTool();
void DrawLine(HDC hdc,POINT coor[2]);
void updateColor(HDC hdc,COLORREF Color,COLORREF ColorIn);
COLORREF GetColorIn();
COLORREF GetColor();
void RegClass(WNDPROC,LPCTSTR);
BOOL intersection(POINT c[4]);
void ShowInputForDrawing();
void MoveConditionInd(int Obj);
void refreshbuttons(HWND hwnd);


// 1 - плюс, 2 - минус, 0 - ничего
int TableConditionContects[10][5] =  {{1,0,1,1,1},
                                      {1,0,1,1,1},
                                      {1,0,2,2,1},
                                      {1,0,2,2,1},
                                      {1,0,2,2,1},
                                      {0,1,0,0,0},
                                      {1,0,2,0,0},
                                      {1,0,2,2,1},
                                      {0,2,2,2,1},
                                      {0,0,0,0,0}};

LPCTSTR TableCondition[10][10] = {{"","set color","","","","","","",""},
                           {"","set color","","","","","","",""},
                           {"","drawing first line","save first, set second","","","","","",""},
                           {"","","","confirm","","","","",""},
                           {"","","","change second","","","","",""},
                           {"","set fill point","set locking","","","","","",""},
                           {"","","","","","close errorwnd", "", "", "close errorwnd"},
                           {"","","","","set line", "", "locking", "draw fill", ""},
                           {"","","","","show errorwnd", "", "show errorwnd", "show errorwnd", ""},
                           {"","quit","quit", "quit","quit","","quit","quit",""}};
int TableMove[10][10] =        {{0,1,0,0,0,0,0,0,0}, 
                                {0,1,0,0,0,0,0,0,0},
                                {0,2,3,0,0,0,0,0,0},
                                {0,0,0,4,0,0,0,0,0},
                                {0,0,0,3,0,0,0,0,0},
                                {0,7,6,0,0,0,0,0,0},
                                {0,0,0,0,0,2,0,0,1},
                                {0,0,0,0,2,0,1,1,0},
                                {0,0,0,0,5,0,5,8,0},
                                {0,-1,-1,-1,-1,0,-1,-1,0}};

int ConditionInd;
int Object;

enum stateDraw{
  Stop,
  One,//Главное окно отображено
  Two,//Добавлен элемент фигуры
  Three,
  Four,
  Five,//Проверка на пересечение сторон(ЛКМ)
  Six,//Отображено окно с ошибкой
  Seven,//Проверка на пересечение сторон(ПКМ)
  Eight,//Проверка курсор внутри или снаружи
  Nine//Отображено окно с ошибкой
};


enum stateDraw Condition;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  hwndMain = CreateWindow("MainWin", "Paint", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 0, 0, 1100, 1040, NULL, NULL, NULL, NULL);
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
      RECT r1;
      GetClientRect(hwnd, &r1);
      int scaling_width = (r1.right)/1000;
      int scaling_height = (r1.bottom)/500;
      RegClass(WndProcChild,"ChildWin");
      refreshbuttons(hwnd);
      break;
    }
    case WM_COMMAND:
    {
        if(LOWORD(wParam) == button_id1)
        {
          RECT r1;
          GetClientRect(hwnd, &r1);
          HWND hwnd1 = CreateWindow("ChildWin", "Window", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 1100, 0, 820, 1040, hwnd, NULL, NULL, NULL);
          ShowWindow(hwnd1, SW_SHOWNORMAL);
        }
        else if(LOWORD(wParam) == button_id2)
        {
          for(int i=0;i<Mainsize;i++)
          {
            MainMas[i].size=0;
            MainMas[i].MassCor->xy.x=0;
            MainMas[i].MassCor->xy.y=0;
            MainMas[i].MassCor->x1y1.x=0;
            MainMas[i].MassCor->x1y1.y=0;
            MainMas[i].DRAW=FALSE;
            MainMas[i].CLOSE=FALSE;
          }
          Mainsize=0;
          ConditionInd=0;
          InvalidateRect(hwnd,NULL,1);
        }
        break;
    }
    case WM_SYSCOMMAND:
    {
      if (wParam == 0xF012)
      {
        return 0;
      }
    }
    case WM_LBUTTONDOWN:
    { 
      Object=2;
      if (ConditionInd==One)
      {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x=LOWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y=HIWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x=0;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y=0;
        MainMas[Mainsize].color=GetColor();
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
        MoveToEx(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y, NULL);
        LineTo(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y);
        DeleteObject(hdc);
        ShowInputForDrawing();
        MoveConditionInd(Object);
      }
      else if(ConditionInd==Two)
      {
        if(MainMas[Mainsize].size!=0)
        {
          MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].x1y1;
        }
        HDC hdc = GetDC (hwnd);
        POINT coordinates[2];
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
        coordinates[0].x=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x;
        coordinates[0].y=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y;
        coordinates[1].x=LOWORD(lParam);
        coordinates[1].y=HIWORD(lParam);
        DrawLine(hdc,coordinates);
        DeleteObject(hdc);
        MoveConditionInd(Object);
      }
      break;
    }
    case WM_LBUTTONUP:
    {
      Object=3;
      if (ConditionInd==Three)
      {
        POINT c[4];
        c[0]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy;
        c[1].x=LOWORD(lParam);
        c[1].y=HIWORD(lParam);
        MoveConditionInd(Object);
        for(int i=0;i<MainMas[Mainsize].size-1;i++)
        {
          c[2]=MainMas[Mainsize].MassCor[i].xy;
          c[3]=MainMas[Mainsize].MassCor[i].x1y1;
          if(intersection(c))
          {
            Object=8;
            MoveConditionInd(Object);
            MessageBox(hwnd,"You crossed the line","Error", MB_OK|MB_APPLMODAL);
            Object=6;
            MoveConditionInd(Object);
            return 1;
          }
        }
        if(MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x==c[1].x||MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y==c[1].y)
        {
          Object=8;
          MoveConditionInd(Object);
          MessageBox(hwnd,"You crossed the line","Error", MB_OK|MB_APPLMODAL);
          Object=6;
          MoveConditionInd(Object);
          return 1;
        }
        Object=7;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x=c[1].x;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y=c[1].y;
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
        MoveToEx(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy.y, NULL);
        LineTo(hdc, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x, MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y);
        MainMas[Mainsize].size++;
        DeleteObject(hdc);
        MoveConditionInd(Object);
      }
      break;
    }
    case WM_MOUSEMOVE:
    {
     Object=4;
      if(ConditionInd==Three)
      {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.x=LOWORD(lParam);
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1.y=HIWORD(lParam);
        InvalidateRect(hwnd,NULL,1);
      }
      break;
    }
    case WM_RBUTTONDOWN:
    {
      Object=5;
      if (ConditionInd==Two)
      {
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy=MainMas[Mainsize].MassCor[MainMas[Mainsize].size-1].x1y1;
        MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1=MainMas[Mainsize].MassCor[0].xy;
        POINT c[4];
        c[0]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].xy;
        c[1]=MainMas[Mainsize].MassCor[MainMas[Mainsize].size].x1y1;
        MoveConditionInd(Object);
        for(int i=1;i<MainMas[Mainsize].size-1;i++)
        {
          c[2]=MainMas[Mainsize].MassCor[i].xy;
          c[3]=MainMas[Mainsize].MassCor[i].x1y1;
          if(intersection(c))
          {
            Object=8;
            MoveConditionInd(Object);
            MessageBox(hwnd,"You crossed the line\nCan't finish","Error", MB_OK|MB_APPLMODAL);
            Object=6;
            MoveConditionInd(Object);
            return 1;
          }
          Object=7;
        }
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainMas[Mainsize].color,GetColorIn());
        DrawLine(hdc,c);
        DeleteObject(hdc);
        Mainsize++;
        ShowInputForDrawing();
        MoveConditionInd(Object);
      }
      else if(ConditionInd==One)
      {
        MoveConditionInd(Object);
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        POINT c[4];
        c[0].x=LOWORD(lParam);
        c[0].y=HIWORD(lParam);
        c[1].x=rcClientRect.right+1;
        c[1].y=rcClientRect.bottom+1;
        int colper=0;
        HDC hdc = GetDC (hwnd);
        BOOL Found=FALSE;
        int f=0;
        for (f;f<Mainsize;f++)
        {
          if(MainMas[f].DRAW==FALSE)
          {
            for(int i=0;i<MainMas[f].size+1;i++)
            {
              c[2]=MainMas[f].MassCor[i].xy;
              c[3]=MainMas[f].MassCor[i].x1y1;
              if(intersection(c))
              {
                colper+=1;
              }
            }
            if(colper%2==1)
            {
              MainMas[f].DRAW=TRUE;
              MainMas[f].colorIn=GetColorIn();
              Object=7;
              MoveConditionInd(Object);
              //Drawer(hdc,MainMas[f].colorIn,f);
              Drawer2(hdc, MainMas[f].colorIn, MainMas[f]);
              DeleteObject(hdc);
              ArchiveMas[Archivesize]=MainMas[f];
              Archivesize++;
              return 1;
            }
            colper=0;
          }
        }
        Object=8;
        MoveConditionInd(Object);
        MessageBox(hwnd,"You didn't get anywhere","Error", MB_OK|MB_APPLMODAL);
        Object=6;
        MoveConditionInd(Object);
      }
      break;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd,&ps);
      if(ConditionInd==Three)
      {
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
          if(MainMas[f].DRAW)
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
      }
      else if (ConditionInd==0)
      {
        ConditionInd=1;
        FillRect(hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 23));
      }
      EndPaint(hwnd,&ps);
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
void ShowInputForDrawing()
{
  if (TableConditionContects[ConditionInd][3]==1)
  {
    for(int i=0;i<16;i++)
    {
      ShowWindow(HWNDMas[i],SW_HIDE);
    }
  }
  else
  {
    for(int i=0;i<16;i++)
    {
      ShowWindow(HWNDMas[i],SW_SHOW);
    }
  }
}

void Drawer(HDC hdc,COLORREF GetColor, int f)
{
  int left = MainMas[f].MassCor[0].xy.x;
  int right = MainMas[f].MassCor[0].xy.x;
  int top = MainMas[f].MassCor[0].xy.y;
  int bot = MainMas[f].MassCor[0].xy.y;
  for (int i=0; i <= MainMas[f].size; i++)
  {
    if (MainMas[f].MassCor[i].xy.x < left) left = MainMas[f].MassCor[i].xy.x;
    else if (MainMas[f].MassCor[i].xy.x > right) right = MainMas[f].MassCor[i].xy.x;
    if (MainMas[f].MassCor[i].xy.y < top) top = MainMas[f].MassCor[i].xy.y;
    else if (MainMas[f].MassCor[i].xy.y > bot) bot = MainMas[f].MassCor[i].xy.y;
  }
  POINT c[4];
  left++;
  top++;
  c[0].x=left;
  c[0].y=top;
  c[1].x=right;
  c[1].y=bot;
  int colper;
  for (int i1 = left; i1 < right; i1++)
  {
    c[0].x=i1;
    for (int j1 = top; j1 < bot; j1++)
    {
      colper=0;
      c[0].y=j1;
      for(int i=0;i<MainMas[f].size+1;i++)
      {
        c[2]=MainMas[f].MassCor[i].xy;
        c[3]=MainMas[f].MassCor[i].x1y1;
        if(intersection(c))
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

void Drawer2(HDC hdc, COLORREF GetColor, struct MainPoly tmp)
{
  for (int dfg = 0; dfg < tmp.size+1; dfg++)
  {
    MoveToEx(hdc, tmp.MassCor[dfg].xy.x, tmp.MassCor[dfg].xy.y, NULL);
    LineTo(hdc, tmp.MassCor[dfg].x1y1.x, tmp.MassCor[dfg].x1y1.y);
  }
  int left = tmp.MassCor[0].xy.x;
  int right = tmp.MassCor[0].xy.x;
  int top = tmp.MassCor[0].xy.y;
  int bot = tmp.MassCor[0].xy.y;
  for (int i=0; i <= tmp.size; i++)
  {
    if (tmp.MassCor[i].xy.x < left) left = tmp.MassCor[i].xy.x;
    else if (tmp.MassCor[i].xy.x > right) right = tmp.MassCor[i].xy.x;
    if (tmp.MassCor[i].xy.y < top) top = tmp.MassCor[i].xy.y;
    else if (tmp.MassCor[i].xy.y > bot) bot = tmp.MassCor[i].xy.y;
  }
  POINT c[4];
  left++;
  top++;
  c[0].x=left;
  c[0].y=top;
  c[1].x=left;
  c[1].y=0;
  int colper;
  for (int i1 = left; i1 < right; i1++)
  {
    c[0].x=i1;
    for (int j1 = top; j1 < bot; j1++)
    {
      colper=0;
      c[0].y=j1;
      for(int i=0;i<tmp.size+1;i++)
      {
        c[2]=tmp.MassCor[i].xy;
        c[3]=tmp.MassCor[i].x1y1;
        if(intersection(c))
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
  for (int dfg = 0; dfg < tmp.size+1; dfg++)
  {
    MoveToEx(hdc, tmp.MassCor[dfg].xy.x, tmp.MassCor[dfg].xy.y, NULL);
    LineTo(hdc, tmp.MassCor[dfg].x1y1.x, tmp.MassCor[dfg].x1y1.y);
  }
}

void MoveConditionInd(int Obj)
{
  ConditionInd = TableMove[Obj][ConditionInd];
}


LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_CREATE:
    {
      EnableWindow(hwndMain, FALSE);
      char str[11],str1[11];
      ListBox = CreateWindow("listbox", NULL,WS_CHILD | WS_VISIBLE | LBS_STANDARD |LBS_WANTKEYBOARDINPUT,20, 20, 250, 630,hwnd, (HMENU) list_id, NULL, NULL);
      for(int i=0;i<Archivesize;i++)
      {
        strcpy(str1,"Figure-");
        sprintf(str, "%d", i+1);
        strcat(str1, str);
        SendMessage(ListBox, LB_ADDSTRING, NULL, str1);
      }
      SetFocus(ListBox);
      if(Archivesize>0)
      {
        DefaultS=TRUE;
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        rcClientRect.left=300;
        InvalidateRect(hwnd,&rcClientRect,1);
        SetFocus(ListBox);
      }
      break;
    }
    case WM_SYSCOMMAND:
    {
      if (wParam == 0xF012)
      {
        return 0;
      }
    }
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd,&ps);
      FillRect(hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 23));
      RECT r2;
      GetClientRect(hwnd, &r2);
      r2.right = 290;
      HBRUSH greenBrush = CreateSolidBrush(RGB(50,50,100));
      FillRect(hdc,&r2,greenBrush);
      if (DefaultS)
      {
        if(ArchiveMas[Ind].DRAW==FALSE)
        {
          updateColor(hdc,ArchiveMas[Ind].color,RGB(185,209,234));
        }
        else
        {
          updateColor(hdc,ArchiveMas[Ind].color,ArchiveMas[Ind].colorIn);
        }
        for(int a=0;a<ArchiveMas[Ind].size+1;a++)
        {
          int x = ArchiveMas[Ind].MassCor[a].xy.x;
          int x1= ArchiveMas[Ind].MassCor[a].x1y1.x;
          int y = ArchiveMas[Ind].MassCor[a].xy.x;
          int y1= ArchiveMas[Ind].MassCor[a].x1y1.y;
        }
        RECT r1;
        GetClientRect(hwnd, &r1);
        float scaling_width = ((float)(r1.right-300))/((float)1100);
        float scaling_height = ((float)(r1.bottom))/((float)1040);
        struct MainPoly Tmp = ArchiveMas[Ind];
        for (int zxc = 0; zxc < Tmp.size+1; zxc++)
        {
          Tmp.MassCor[zxc]=ArchiveMas[Ind].MassCor[zxc];
          Tmp.MassCor[zxc].xy.x=scaling_width*(Tmp.MassCor[zxc].xy.x)+300;
          Tmp.MassCor[zxc].xy.y=scaling_height*(Tmp.MassCor[zxc].xy.y);
          Tmp.MassCor[zxc].x1y1.x=scaling_width*(Tmp.MassCor[zxc].x1y1.x)+300;
          Tmp.MassCor[zxc].x1y1.y=scaling_height*(Tmp.MassCor[zxc].x1y1.y);
        }
        Drawer2(hdc, Tmp.colorIn, Tmp);

      }
      EndPaint(hwnd,&ps);
      break;
    }
    case WM_VKEYTOITEM:
    {
      if (wParam==VK_ESCAPE)
      {
        EnableWindow(hwndMain, TRUE);
        Entry=FALSE;
        DestroyWindow(hwnd);
        break;
      }
      else if (LOWORD(wParam) == VK_UP)
      {
        printf("%d",SendMessage(ListBox, LB_GETCOUNT, NULL,NULL));
        DefaultS=TRUE;
        Ind = SendMessage(ListBox, LB_GETCARETINDEX, NULL,NULL);
        printf("%d",Ind);
        if (Ind>0)
        {
          Ind--;

        }
        else
        {
          MessageBox(hwnd,"You didn't get anywhere","Error", MB_OK|MB_APPLMODAL);
        }
        Entry=TRUE;
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        rcClientRect.left=300;
        InvalidateRect(hwnd,&rcClientRect,1);
        SetFocus(ListBox);
      }    
      else if  (LOWORD(wParam) == VK_DOWN)
      {
        printf("%d",SendMessage(ListBox, LB_GETCOUNT, NULL,NULL));
        DefaultS=TRUE;
        Ind = SendMessage(ListBox, LB_GETCARETINDEX, NULL,NULL);
        if (Entry)
        {
          if (SendMessage(ListBox, LB_GETCOUNT, NULL,NULL)>Ind+1)
          {
            Ind++;
          }
          else
          {
            MessageBox(hwnd,"You didn't get anywhere","Error", MB_OK|MB_APPLMODAL);
          }
        }
        Entry=TRUE;
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        rcClientRect.left=300;
        InvalidateRect(hwnd,&rcClientRect,1);
        SetFocus(ListBox);
      } 
      else if(LOWORD(wParam) ==VK_RETURN)
      {
        HDC hdc = GetDC (hwndMain);
        POINT ptPoints[ArchiveMas[Ind].size+1];
        for(int a=0;a<ArchiveMas[Ind].size+1;a++)
        {
          ptPoints[a].x=ArchiveMas[Ind].MassCor[a].xy.x;
          ptPoints[a].y=ArchiveMas[Ind].MassCor[a].xy.y;
        }
        if(ArchiveMas[Ind].DRAW==FALSE)
        {
          updateColor(hdc,ArchiveMas[Ind].color,RGB(185,209,234));
        }
        else
        {
          updateColor(hdc,ArchiveMas[Ind].color,ArchiveMas[Ind].colorIn);
        }
        struct MainPoly tmp = ArchiveMas[Ind];
        Drawer2(hdc,ArchiveMas[Ind].colorIn,tmp);

        MainMas[Mainsize]=ArchiveMas[Ind];
        Mainsize++;
        RECT rcClientRect;
        GetClientRect(hwndMain, &rcClientRect);
        rcClientRect.top=60;
        InvalidateRect(hwndMain,&rcClientRect,1);
        SetFocus(ListBox);
      }   
      break;
    }
    case WM_MOVE:
    {
      SetFocus(ListBox);
      return 0;
    }
    case WM_CLOSE:
    {
      EnableWindow(hwndMain, TRUE);
      Entry=FALSE;
      DestroyWindow(hwnd);
      break;
    }
    default:
      return DefWindowProcA(hwnd, msg, wParam, lParam);
  }
  return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void refreshbuttons(HWND hwnd)
{
  RECT r1;
      GetClientRect(hwnd, &r1);
      float scaling_width = ((float)(r1.right))/((float)1100);
      float scaling_height = ((float)(r1.bottom))/((float)1040);
      for (int asd = 0; asd < 16; asd++)
      {
        DestroyWindow(HWNDMas[asd]);
      }
      HWNDMas[0] = CreateWindow("static", "Color RGB", WS_VISIBLE | WS_CHILD, scaling_width*80, 15, 75, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[1] = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, scaling_width*50, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[2] = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, scaling_width*100, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[3] = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, scaling_width*150, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[4] = CreateWindow("static", "Color RGB in", WS_VISIBLE | WS_CHILD, scaling_width*915, 15, 90, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[5] = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, scaling_width*885, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[6] = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, scaling_width*935, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[7] = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, scaling_width*985, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWNDMas[8] = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , scaling_width*65, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[9] = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , scaling_width*115, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[10]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , scaling_width*165, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[11]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , scaling_width*900, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[12]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , scaling_width*950, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[13]= CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , scaling_width*1000, 38, 30, 20, hwnd, NULL, NULL, NULL);
      HWNDMas[14] =  CreateWindow("button", "Archive", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , scaling_width*430, 38, 100, 20, hwnd, (HMENU)button_id1, NULL, NULL);
      HWNDMas[15] =  CreateWindow("button", "Clear", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , scaling_width*570, 38, 100, 20, hwnd, (HMENU)button_id2, NULL, NULL);
}