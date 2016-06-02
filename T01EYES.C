#include <windows.h>
#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrewInstance, CHAR *CmdLine, INT ShowCmd )
{ 
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
 

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0; 
  wc.cbWndExtra = 0; 
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0; 
  }

  hWnd = CreateWindow(WND_CLASS_NAME, 
           "Tittle",
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT, CW_USEDEFAULT,
           CW_USEDEFAULT, CW_USEDEFAULT,
           NULL,
           NULL,
           hInstance,
           NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {  
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static INT w, h;
  HDC hDC;
  PAINTSTRUCT ps;
  HDC hMemDC;
  HDC hBm;
 

  switch (Msg)
  {
  case WM_DESTROY:
    DeleteObject(hMemDC);
    DeleteObject(hDC);
    DeleteObject(hBm);
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC (hWnd, hDC);
    SelectObject(hMemDC, hDC);
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 30, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    DrawEye(100, 100, 100, 50);
    return 0;



  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, FALSE);
    break; 
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);           
    
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}


VOID DrawEye( HWND hWnd, INT x, INT y, INT R1, INT R2 )
{
  DOUBLE d, Xcoordinate, Ycoordinate, t; 
  INT dx, dy;
  POINT pt;
  HDC hDC;
  
  GetCursorPos(&pt); 
  ScreenToClient(hWnd, &pt);
   d = (R1 - R2) / sqrt((pt.x - x) * (pt.x - x) + (pt.y - y) * (pt.y - y)) ;
   dx = pt.x - x;
   dy = pt.y - y;
   if (d < 1)
   { 
     dx *= t;
     dy *= t;
   }
   d = (R1 - R2) / sqrt((pt.x - x) * (pt.x - x) + (pt.y - y) * (pt.y - y)) ;
   Ycoordinate = y - dy * d;
   Xcoordinate =  x + dx * d;
   Ellipse(hDC, Xcoordinate - R2, Ycoordinate - R2, Xcoordinate + R2, Ycoordinate + R2);
   Ellipse(hDC, x - R1, y - R1, x + R1, y + R1);
   

}