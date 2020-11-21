#include <windows.h>
#include <stdio.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define edit_id1 1
#define edit_id2 2
#define edit_id3 3
#define edit_id4 4
#define edit_id5 5
#define edit_id6 6
#define button_id1 7
#define button_id2 8
#define button_id3 9
#define list_id 10
HPEN pen;
HBRUSH brush;
HWND hwndMain;
HWND edit1;
HWND edit2;
HWND edit3;
HWND edit4;
HWND edit5;
HWND edit6;
HWND ListBox;
HWND DRAW_btn;
int flag=0;
int pupu=1;
BOOL Entry=FALSE;
struct saveCor
{
  int x;
  int y;
  int x1;
  int y1;
  COLORREF color;
};

struct MainPoly
{
  struct saveCor MassCor[100];
  BOOL CLOSE;
  BOOL DRAW;
  int size;
  COLORREF colorIn;
};
int amount;
struct MainPoly MainMas[100];

struct MainPoly MainE;
int Ind;
int Mainsize=0;
void Drawer(HDC hdc,int x,int y,COLORREF GetColor,COLORREF GetColorIn);
void FreeTool();
void DrawLine(HDC hdc,POINT coor[2]);
void updateColor(HDC hdc,COLORREF Color,COLORREF ColorIn);
COLORREF GetColorIn();
COLORREF GetColor();
void RegClass(WNDPROC,LPCTSTR);
BOOL intersection(POINT c[4]);
BOOL DefaultS=FALSE;
int flag34=0;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  RegClass(WndProc,"MainWin");
  hwndMain = CreateWindow("MainWin", "Main Window", WS_OVERLAPPEDWINDOW, 10, 10, 1100, 720, NULL, NULL, NULL, NULL);
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
      RegClass(WndProcChild,"ChildWin");
      MainE.size=0;
      edit1 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 65, 38, 30, 20, hwnd, (HMENU)edit_id1, NULL, NULL);
      edit2 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 115, 38, 30, 20, hwnd, (HMENU)edit_id2, NULL, NULL);
      edit3 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 165, 38, 30, 20, hwnd, (HMENU)edit_id3, NULL, NULL);
      edit4 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 900, 38, 30, 20, hwnd, (HMENU)edit_id4, NULL, NULL);
      edit5 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 950, 38, 30, 20, hwnd, (HMENU)edit_id5, NULL, NULL);
      edit6 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER , 1000, 38, 30, 20, hwnd, (HMENU)edit_id6, NULL, NULL);
      HWND textMain = CreateWindow("static", "Color RGB", WS_VISIBLE | WS_CHILD, 80, 15, 75, 20, hwnd, NULL, NULL, NULL);
      HWND R = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, 50, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND G = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, 100, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND B = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, 150, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND textMain_In = CreateWindow("static", "Color RGB in", WS_VISIBLE | WS_CHILD, 915, 15, 90, 20, hwnd, NULL, NULL, NULL);
      HWND R_In = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, 885, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND G_In = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, 935, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND B_In = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, 985, 40, 12, 15, hwnd, NULL, NULL, NULL);
      HWND buttonDownload =  CreateWindow("button", "Download", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 430, 38, 100, 20, hwnd, (HMENU)button_id1, NULL, NULL);
      HWND buttonSave =  CreateWindow("button", "Save", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 570, 38, 100, 20, hwnd, (HMENU)button_id2, NULL, NULL);

      break;
    }
    case WM_COMMAND:
    {
        if(LOWORD(wParam) == button_id1)
        {
            HWND hwnd1 = CreateWindow("ChildWin", "Window", WS_OVERLAPPEDWINDOW  , 700, 150, 1000, 500, hwnd, NULL, NULL, NULL);
            ShowWindow(hwnd1, SW_SHOWNORMAL);
        }
        else if(LOWORD(wParam) == button_id2)
        {
          if(MainE.CLOSE==TRUE)
          {
            MainMas[Mainsize]=MainE;
            Mainsize++;
          }
          else
          {
            MessageBox(hwnd,"Noooo","Error4", MB_OK);
          }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
      if (pupu==1)
      {
        MainE.CLOSE=FALSE;
        MainE.colorIn=0;
        MainE.DRAW=FALSE;
        MainE.size=0;
        pupu=0;
      }
      flag34=0;
      flag=1;
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      if (MainE.size!=0)
      {
        MainE.MassCor[MainE.size].x=MainE.MassCor[MainE.size-1].x1;
        MainE.MassCor[MainE.size].y=MainE.MassCor[MainE.size-1].y1;
        MainE.MassCor[MainE.size].color=GetColor();
      }
      else
      {
        MainE.MassCor[MainE.size].x=x;
        MainE.MassCor[MainE.size].y=y;
        MainE.MassCor[MainE.size].color=GetColor();
      }
      break;
    }
    case WM_LBUTTONUP:
    {
      flag=2;
      POINT c[4];
      c[0].x=-MainE.MassCor[MainE.size].x;
      c[0].y=-MainE.MassCor[MainE.size].y;
      c[1].x=-MainE.MassCor[MainE.size].x1;
      c[1].y=-MainE.MassCor[MainE.size].y1;
      for(int i=0;i<MainE.size-1;i++)
      {
        c[2].x=-MainE.MassCor[i].x;
        c[2].y=-MainE.MassCor[i].y;
        c[3].x=-MainE.MassCor[i].x1;
        c[3].y=-MainE.MassCor[i].y1;
        if(intersection(c)==TRUE)
        {
          MessageBox(hwnd,"You crossed the line","Error", MB_OK);
          MainE.size--;
          MainE.size++;
          flag=1;
          return 1;
        }
      }
      MainE.size++;
      break;
    }
    case WM_MOUSEMOVE:
    {
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      if(flag==1)
      {
        MainE.MassCor[MainE.size].x1=x;
        MainE.MassCor[MainE.size].y1=y;
      }
      RECT rcClientRect;
      GetClientRect(hwnd, &rcClientRect);
      rcClientRect.top=60;
      InvalidateRect(hwnd,&rcClientRect,1);
      break;
    }
    case WM_RBUTTONDOWN:
    {
      if (flag34==0)
      {
        MainE.MassCor[MainE.size].x=MainE.MassCor[MainE.size-1].x1;
        MainE.MassCor[MainE.size].y=MainE.MassCor[MainE.size-1].y1;
        MainE.MassCor[MainE.size].x1=MainE.MassCor[0].x;
        MainE.MassCor[MainE.size].y1=MainE.MassCor[0].y;
        MainE.MassCor[MainE.size].color=GetColor();
        flag=2;
        POINT c[4];
        c[0].x=-MainE.MassCor[MainE.size].x;
        c[0].y=-MainE.MassCor[MainE.size].y;
        c[1].x=-MainE.MassCor[MainE.size].x1;
        c[1].y=-MainE.MassCor[MainE.size].y1;

        for(int i=1;i<MainE.size-1;i++)
        {
          c[2].x=-MainE.MassCor[i].x;
          c[2].y=-MainE.MassCor[i].y;
          c[3].x=-MainE.MassCor[i].x1;
          c[3].y=-MainE.MassCor[i].y1;
          if(intersection(c)==TRUE)
          {
            MessageBox(hwnd,"You crossed the line\nCan't finish","Error2", MB_OK);
            MainE.size--;
            MainE.size++;
            flag=1;
            return 1;
          }
        }
        POINT coordinates[2];
        coordinates[0].x=MainE.MassCor[MainE.size].x;
        coordinates[0].y=MainE.MassCor[MainE.size].y;
        coordinates[1].x=MainE.MassCor[MainE.size].x1;
        coordinates[1].y=MainE.MassCor[MainE.size].y1;
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,GetColor(),GetColorIn());
        POINT ptPoints[MainE.size+1];
        for(int i=0;i<MainE.size+1;i++)
        {
          ptPoints[i].x=MainE.MassCor[i].x;
          ptPoints[i].y=MainE.MassCor[i].y;
        }
        DrawLine(hdc,coordinates);
        MainE.CLOSE=TRUE;
        pupu=1;
        DeleteObject(hdc);
        flag34=1;
      }
      else
      {
        int colper=0;
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        POINT c[4];
        c[0].x=x;
        c[0].y=y;
        c[1].x=1100;
        c[1].y=720;
        for(int i=0;i<MainE.size+1;i++)
        {
            c[2].x=MainE.MassCor[i].x;
            c[2].y=MainE.MassCor[i].y;
            c[3].x=MainE.MassCor[i].x1;
            c[3].y=MainE.MassCor[i].y1;
            if(intersection(c)==TRUE)
            {
              colper+=1;
              flag=2;
            }  
        }
        HDC hdc = GetDC (hwnd);
        updateColor(hdc,MainE.MassCor->color,RGB(185,209,234));
        POINT ptPoints[MainE.size+1];
        for(int a=0;a<MainE.size+1;a++)
        {
          ptPoints[a].x=MainE.MassCor[a].x;
          ptPoints[a].y=MainE.MassCor[a].y;
        }
        Polygon(hdc, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
        if(colper %2 == 1)
        {
          HDC hdc = GetDC (hwnd);
          c[0].x=-x;
          c[0].y=-y;
          c[1].x=-2000;
          c[1].y=-2000;
          colper=0;
          int i1=0;
            for(int i=0;i<MainE.size+1;i++)
            {
              c[2].x=-MainE.MassCor[i].x;
              c[2].y=-MainE.MassCor[i].y;
              c[3].x=-MainE.MassCor[i].x1;
              c[3].y=-MainE.MassCor[i].y1;
              if(intersection(c)==TRUE)
              {
                colper+=1;
                flag=2;
              }
            }
            if(colper==1&&MainE.DRAW==FALSE)
            {
              MainE.DRAW=TRUE;
              MainE.colorIn=GetColorIn();
              break;
            }
            colper=0;
          
          int x = LOWORD(lParam);
          int y = HIWORD(lParam);
          updateColor(hdc,MainE.MassCor->color,RGB(185,209,234));
          POINT ptPoints[MainE.size+1];
          for(int a=0;a<MainE.size+1;a++)
          {
            ptPoints[a].x=MainE.MassCor[a].x;
            ptPoints[a].y=MainE.MassCor[a].y;
          }
          Polygon(hdc, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
          Drawer(hdc,x,y,GetColor(),GetColorIn());
          flag=2;
          DeleteObject(hdc);
        }
        else
        {
          MessageBox(hwnd,"You didn't get anywhere","Error3", MB_OK);
        }
      }
      break;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      if((flag==1))
      {
        HDC hdc = BeginPaint(hwnd,&ps);
        HDC memDC = CreateCompatibleDC(hdc);
        RECT rcClientRect;
        GetClientRect( hwnd, &rcClientRect );
        HBITMAP bmp = CreateCompatibleBitmap( hdc,  rcClientRect.right , rcClientRect.bottom );
        SelectObject(memDC,bmp);
        FillRect(memDC,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 23));
        POINT coordinates[2];
        for(int i=0;i<MainE.size+1;i++)
        {
            coordinates[0].x=MainE.MassCor[i].x;
            coordinates[0].y=MainE.MassCor[i].y;
            coordinates[1].x=MainE.MassCor[i].x1;
            coordinates[1].y=MainE.MassCor[i].y1;
            updateColor(memDC,MainE.MassCor[i].color,MainE.colorIn);
            if (i!=0)
            {
                coordinates[0].x=MainE.MassCor[i-1].x1;
                coordinates[0].y=MainE.MassCor[i-1].y1;
                DrawLine(memDC,coordinates);
            }
            else
            {
                DrawLine(memDC,coordinates);
            }
                DrawLine(memDC,coordinates);
        }
        if(MainE.DRAW==TRUE)
        {
            updateColor(hdc,MainE.MassCor->color,MainE.colorIn);
            POINT ptPoints[MainE.size+1];
            for(int a=0;a<MainE.size+1;a++)
            {
                ptPoints[a].x=MainE.MassCor[a].x;
                ptPoints[a].y=MainE.MassCor[a].y;
            }
            Polygon(memDC, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
        }
        BitBlt(hdc,0,0,rcClientRect.right,rcClientRect.bottom,memDC,0,0,SRCCOPY);
        DeleteObject(bmp);
        DeleteDC(memDC);
        EndPaint(hwnd,&ps);
    }
    else if (flag==0)
    {
        HDC hdc = BeginPaint(hwnd,&ps);
        FillRect(hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 23));
        POINT ptPoints[MainE.size+1];
        for(int a=0;a<MainE.size+1;a++)
        {
          ptPoints[a].x=MainE.MassCor[a].x;
          ptPoints[a].y=MainE.MassCor[a].y;
        }
        if(MainE.DRAW==FALSE)
        {
          updateColor(hdc,MainE.MassCor->color,RGB(185,209,234));
        }
        else
        {
          updateColor(hdc,MainE.MassCor->color,MainE.colorIn);
        }
        Polygon(hdc, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
            


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
  double ax1=c[0].x;
  double ay1=c[0].y;
  double ax2=c[1].x;
  double ay2=c[1].y;
  double bx1=c[2].x;
  double by1=c[2].y;
  double bx2=c[3].x;
  double by2=c[3].y;
  double v1=(bx2-bx1)*(ay1-by1)-(by2-by1)*(ax1-bx1);
  double v2=(bx2-bx1)*(ay2-by1)-(by2-by1)*(ax2-bx1);
  double v3=(ax2-ax1)*(by1-ay1)-(ay2-ay1)*(bx1-ax1);
  double v4=(ax2-ax1)*(by2-ay1)-(ay2-ay1)*(bx2-ax1);
  return ((v1*v2<=0) && (v3*v4<=0));
}

COLORREF GetColorIn()
{
  CHAR editText1[1],editText2[1],editText3[1];
  GetWindowText(edit1, editText1 ,4);
  int REDIT=atoi (editText1);
  REDIT%256;
  GetWindowText(edit2, editText2 ,4);
  int GEDIT=atoi (editText2);
  GEDIT%256;
  GetWindowText(edit3, editText3 ,4);
  int BEDIT=atoi (editText3);
  BEDIT%256;
  COLORREF Color = RGB(REDIT,GEDIT,BEDIT);
  return Color;
}

COLORREF GetColor()
{
  CHAR editText1[1],editText2[1],editText3[1];
  GetWindowText(edit4, editText1 ,4);
  int REDIT=atoi (editText1);
  REDIT%256;
  GetWindowText(edit5, editText2 ,4);
  int GEDIT=atoi (editText2);
  GEDIT%256;
  GetWindowText(edit6, editText3 ,4);
  int BEDIT=atoi (editText3);
  BEDIT%256;
  COLORREF Color = RGB(REDIT,GEDIT,BEDIT);
  return Color;
}

void FreeTool() 
{
  DeleteObject(pen);
  DeleteObject(brush);
}

void updateColor(HDC hdc,COLORREF Color,COLORREF ColorIn) 
{
  FreeTool();
  pen = CreatePen(PS_SOLID, 10, Color);
  brush = CreateSolidBrush(ColorIn);
  SelectObject(hdc, pen);
  SelectObject(hdc, brush);
}

void Drawer(HDC hdc,int x,int y,COLORREF GetColor,COLORREF GetColorIn)
{
  COLORREF Background = GetPixel(hdc,x,y);
  SetPixel(hdc,x,y,GetColorIn);
  if (Background==GetColorIn||Background==GetColor)
  {
    return;
  }
  Drawer(hdc,x,y-1,GetColor,GetColorIn);
  Drawer(hdc,x-1,y,GetColor,GetColorIn);
  Drawer(hdc,x,y+1,GetColor,GetColorIn);
  Drawer(hdc,x+1,y,GetColor,GetColorIn);
}

LRESULT CALLBACK WndProcChild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_CREATE:
    {
      char str[11],str1[11];
      ListBox = CreateWindow("listbox", NULL,WS_CHILD | WS_VISIBLE | LBS_STANDARD |LBS_WANTKEYBOARDINPUT,30, 20, 300, 400,hwnd, (HMENU) list_id, NULL, NULL);
      HWND DRAW_btn =  CreateWindow("button", "DRAW", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON , 130, 425, 100, 20, hwnd, (HMENU)button_id3, NULL, NULL);
      for(int i=0;i<Mainsize;i++)
      {
        strcpy(str1,"Figure-");
        sprintf(str, "%d", i+1);
        strcat(str1, str);
        SendMessage(ListBox, LB_ADDSTRING, NULL, str1);
      }
      amount = SendMessage(ListBox, LB_GETCOUNT, NULL,NULL);
      SetFocus(ListBox);
      break;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd,&ps);
      FillRect(hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 23));
      if (DefaultS)
      {
        int minX=1100,minY=720;
        if(MainMas[Ind].DRAW==FALSE)
        {
          updateColor(hdc,MainMas[Ind].MassCor->color,RGB(185,209,234));
        }
        else
        {
          updateColor(hdc,MainMas[Ind].MassCor->color,MainMas[Ind].colorIn);
        }
        for(int a=0;a<MainMas[Ind].size+1;a++)
        {
          int x = MainMas[Ind].MassCor[a].x;
          int x1= MainMas[Ind].MassCor[a].x1;
          int y = MainMas[Ind].MassCor[a].y;
          int y1= MainMas[Ind].MassCor[a].y1;
          if (x<minX)
          {
            minX=x;
          }
          if (x1<minX)
          {
            minX=x1;
          }
          if (y<minY)
          {
            minY=y;
          }
          if (y1<minY)
          {
            minY=y1;
          }

        }
        POINT ptPoints[MainMas[Ind].size+1];
        for(int a=0;a<MainMas[Ind].size+1;a++)
        {
          ptPoints[a].x=MainMas[Ind].MassCor[a].x-minX+400;
          ptPoints[a].y=MainMas[Ind].MassCor[a].y-minY+100;
        }
        Polygon(hdc, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
        
      }
      EndPaint(hwnd,&ps);
      break;
    }
    case WM_COMMAND:
    {
      if (LOWORD(wParam)==button_id3)
      {
        HDC hdc = GetDC (hwndMain);
        POINT ptPoints[MainMas[Ind].size+1];
        for(int a=0;a<MainMas[Ind].size+1;a++)
        {
          ptPoints[a].x=MainMas[Ind].MassCor[a].x;
          ptPoints[a].y=MainMas[Ind].MassCor[a].y;
        }
        if(MainMas[Ind].DRAW==FALSE)
        {
          updateColor(hdc,MainMas[Ind].MassCor->color,RGB(185,209,234));
        }
        else
        {
          updateColor(hdc,MainMas[Ind].MassCor->color,MainMas[Ind].colorIn);
        }
        Polygon(hdc, ptPoints,sizeof ptPoints / sizeof ptPoints[0]);
        MainE=MainMas[Ind];
        RECT rcClientRect;
        GetClientRect(hwndMain, &rcClientRect);
        rcClientRect.top=60;
        InvalidateRect(hwndMain,&rcClientRect,1);
        SetFocus(ListBox);
      }
      break;
    }
    case WM_VKEYTOITEM:
    {
      if (LOWORD(wParam) == VK_UP)
      {
        DefaultS=TRUE;
        Ind = SendMessage(ListBox, LB_GETCARETINDEX, NULL,NULL);
        if (Ind>0)
        {
          Ind--;
        }
        printf("UP:%d\n",Ind);
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        rcClientRect.left=335;
        InvalidateRect(hwnd,&rcClientRect,1);
        SetFocus(ListBox);
      }    
      else if  (LOWORD(wParam) == VK_DOWN)
      {
        DefaultS=TRUE;
        Ind = SendMessage(ListBox, LB_GETCARETINDEX, NULL,NULL);
        if (Entry&&amount>Ind+1)
        {
          Ind++;
        }
        else
        {
          Entry=TRUE;
        }
        
        printf("DOWN:%d\n",Ind);
         printf("amount:%d\n",amount);
        /* Ind--;
         printf("\nMAinDOWN:%d\n",Ind);*/
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        rcClientRect.left=335;
        InvalidateRect(hwnd,&rcClientRect,1);
        SetFocus(ListBox);
      }    
      /*if (LOWORD(wParam) == VK_RETURN)
      {
        DefaultS=TRUE;
        Ind = SendMessage(ListBox, LB_GETCARETINDEX, NULL,NULL);
        RECT rcClientRect;
        GetClientRect(hwnd, &rcClientRect);
        rcClientRect.left=335;
        InvalidateRect(hwnd,&rcClientRect,1);
        SetFocus(ListBox);
      }*/
      break;
    }
    case WM_MOVE:
    {
      RECT rcClientRect;
      GetClientRect(hwnd, &rcClientRect);
      rcClientRect.top=60;
      InvalidateRect(hwndMain,&rcClientRect,1);
      flag=0;
      SetFocus(ListBox);
      break;
    }
    case WM_CLOSE:
    {
      Entry=FALSE;
      DestroyWindow(hwnd);
      break;
    }
    default:
      return DefWindowProcA(hwnd, msg, wParam, lParam);
  }
  return DefWindowProcA(hwnd, msg, wParam, lParam);
}