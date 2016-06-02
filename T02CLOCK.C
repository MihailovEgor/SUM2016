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
  HDC hDC, hMemDC;
  PAINTSTRUCT ps;
  static HBITMAP  hBm;
  static BITMAP bm;
 
  
  
  switch (Msg)
  {
  case WM_DESTROY:
    DeleteObject(hDC);
    DeleteObject(hBm);
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;                                 
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (hDC != NULL)                          
      DeleteObject(hDC);
    hMemDC = CreateCompartibleDC(hDC);         
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_CREATE:   
    hBm = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SetTimer(hWnd, 30, 30, NULL);    
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    GetObject(hBm, sizeof(bm), &bm);
    BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);                       
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, FALSE);
    break; 
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);               
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}