/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2019 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 5.06.1                                           *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : SIM_OS.c
Purpose : Collection of functions to simulate the look & feel of the
          target device.
*/

#include <Windows.h>
#include <stdio.h>
#include "SIM_OS_Resource.h"
#include "SIM_OS.h"
#include "BSP.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#define SIM_OS_TRANSPARENT_COLOR  (0xFF0000)
#define SIM_OS_TIMER_PERIOD       (0)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static HWND        _hWnd           = NULL;
static HINSTANCE   _hInst          = NULL;
static HBITMAP     _hBmpDevice     = NULL;
static HMENU       _hMenuPopup     = NULL;
static HANDLE      _hWindowThread  = NULL;
static HANDLE      _hInitEvent     = NULL;
static const int   _rgbTransparent = SIM_OS_TRANSPARENT_COLOR;
static const char  _sWindowClass[] = "embOS device";

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _MessageBox()
*/
static void _MessageBox(char* sText) {
  MessageBoxA(_hWnd, sText, "embOS Simulation", MB_OK | MB_ICONEXCLAMATION);
}

/*********************************************************************
*
*       _ErrorWin32()
*/
static void _ErrorWin32(char* sText) {
  DWORD LastError;
  char  sError[128];

  LastError = GetLastError();
  if (sText == NULL) {
    sprintf(sError, "Win32 Error Code: %d\n", (int)LastError);
  } else {
    sprintf(sError, "Win32 Error Code: %d\n%s\n", (int)LastError, sText);
  }
  _MessageBox(sError);
  ExitProcess(-1);
}

/*********************************************************************
*
*       _DrawRect()
*/
static void _DrawRect(HDC hDC, int x, int y, int w, int h, COLORREF Color) {
  HBRUSH hBrush;
  HPEN   hPen;

  hBrush = CreateSolidBrush(Color);
  hPen   = CreatePen(PS_SOLID, 1, Color);
  SelectObject(hDC, hBrush);
  SelectObject(hDC, hPen);
  Rectangle(hDC, x, y, x + w - 1, y + h - 1);
  DeleteObject(hPen);
  DeleteObject(hBrush);
}

/*********************************************************************
*
*       _MixColors()
*/
static COLORREF _MixColors(COLORREF Color, COLORREF BkColor, unsigned Intens) {
  unsigned int R;
  unsigned int G;
  unsigned int B;

  //
  //  Calc Color separations for FgColor first
  //
  R = (Color & 0xff0000) * Intens;
  G = (Color & 0x00ff00) * Intens;
  B = (Color & 0x0000ff) * Intens;
  //
  //  Add Color separations for BkColor
  //
  Intens = 256 - Intens;
  R     += (BkColor & 0xff0000) * Intens;
  G     += (BkColor & 0x00ff00) * Intens;
  B     += (BkColor & 0x0000ff) * Intens;
  R      = (R >> 8) & 0xff0000;
  G      = (G >> 8) & 0x00ff00;
  B      = (B >> 8);
  return R + G + B;
}

/*********************************************************************
*
*       _DrawLED()
*/
static void _DrawLED(HDC hDC, int x, int y, int w, int h, COLORREF Color) {
  int Light;
  int R;
  int G;
  int B;

  R     = (Color & 0xFF0000) >> 16;
  G     = (Color & 0x00FF00) >>  8;
  B     = (Color & 0x0000FF);
  Light = ((R + G + B) > 0xA0) ? 1 : 0;
  _DrawRect(hDC, x,     y,     w,     h,     0);
  _DrawRect(hDC, x + 1, y + 1, w - 2, h - 2, _MixColors(0x000000, Color, 100));
  _DrawRect(hDC, x + 2, y + 2, w - 4, h - 4, _MixColors(0x000000, Color,  50));
  _DrawRect(hDC, x + 2, y + 3, w - 4, h - 6, Color);
  _DrawRect(hDC, x + 2, y + 5, w - 4, h -10, _MixColors(0xFFFFFF, Color, Light ?  90 : 40));
  _DrawRect(hDC, x + 3, y + 7, w - 6, h -14, _MixColors(0xFFFFFF, Color, Light ? 150 : 75));
}


#if defined(SIM_OS_TIMER_PERIOD) && (SIM_OS_TIMER_PERIOD != 0)

/*********************************************************************
*
*       _cbTimer()
*
*  Function description
*    This is a timer callback routine.
*    It is periodically called by windows (through a WM_TIMER message).
*    We use it to periodically invalidate the main window.
*/
void CALLBACK _cbTimer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
  OS_USEPARA(uMsg);
  OS_USEPARA(idEvent);
  OS_USEPARA(dwTime);
  OS_USEPARA(hWnd);
  OS_SIM_Update();
}

#endif

/*********************************************************************
*
*       _cbAbout()
*
*  Function description
*    Implementation of About window callback.
*    Acts according to the received message (if valid).
*
*  Parameters
*    hDlg   : A handle to the window procedure that received the message.
*    message: The message.
*    wParam : Additional message information. The content of this parameter depends on the value of the message parameter.
*    lParam : Additional message information. Not used at this time.
*
*  Return value
*    == 0: Action was not performed (invalid action / invalid user command).
*    == 1: Action was performed successfully.
*/
static LRESULT CALLBACK _cbAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  OS_USEPARA(lParam);
  switch (message) {
  case WM_INITDIALOG:
  {
    char acBuffer[256];

    strcpy(acBuffer, "Compiled " __TIME__ " on " __DATE__);
    SetWindowText(GetDlgItem(hDlg, SIM_OS_ID_DATE), acBuffer);
    return TRUE;
  }
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return TRUE;
    }
    break;
  default:
    break;
  }
  return FALSE;
}

/*********************************************************************
*
*       _GetWidthOfBitmap()
*
*  Function description
*    Evaluates the width of a bitmap.
*
*  Parameters
*    hBmp: A handle to the bitmap.
*
*  Return value
*    Width of the specified bitmap as an integer.
*/
static int _GetWidthOfBitmap(HBITMAP hBmp) {
  BITMAP bmp;

  GetObject(hBmp, sizeof(bmp), &bmp);
  return bmp.bmWidth;
}

/*********************************************************************
*
*       _GetYSizeOfBitmap()
*
*  Function description
*    Evaluates the height of a bitmap.
*
*  Parameters
*    hBmp: A handle to the bitmap.
*
*  Return value
*    Height of the specified bitmap as an integer.
*/
static int _GetYSizeOfBitmap(HBITMAP hBmp) {
  BITMAP bmp;

  GetObject(hBmp, sizeof(bmp), &bmp);
  return bmp.bmHeight;
}

/*********************************************************************
*
*       _GetScanLine()
*
*  Function description
*    Retrieves the bits of the specified line of a bitmap and copies them into a buffer as a DIB.
*
*  Parameters
*    hBmp: A handle to the bitmap. This must be a compatible bitmap (DDB).
*    y   : The first scan line to retrieve.
*    pRGB: A pointer to a buffer to receive the bitmap data.
*/
static void _GetScanLine(HBITMAP hBmp, int y, DWORD* pRGB) {
  if (hBmp) {
    BITMAPV4HEADER BmpInfo = { 0 };
    HDC            hdc;

    BmpInfo.bV4Size = sizeof(BmpInfo);
    hdc             = GetWindowDC(NULL);
    //
    // Fill in the Bitmap info structure
    //
    GetDIBits(hdc, hBmp, 0, 0, NULL, (BITMAPINFO*)&BmpInfo, DIB_RGB_COLORS);
    BmpInfo.bV4V4Compression = BI_RGB;
    BmpInfo.bV4BitCount = 32;
    if (BmpInfo.bV4Height > 0) {
      y = BmpInfo.bV4Height - y - 1;
    }
    GetDIBits(hdc, hBmp, y, 1, pRGB, (BITMAPINFO*)&BmpInfo, DIB_RGB_COLORS);
  }
}

/*********************************************************************
*
*       _ComparePixels()
*
*  Function description
*    Evaluates whether two specified colors are identical.
*
*  Parameters
*    Color0: RGB representation of the first color.
*    Color1: RGB representation of the second color.
*
*  Return value
*    == 0: Colors match.
*    != 0: Colors do not match.
*/
static int _ComparePixels(OS_U32 Color0, OS_U32 Color1) {
  return ((Color0 & 0xffffff) ^ (Color1 & 0xffffff));
}

/*********************************************************************
*
*       _SetBitmapRegion()
*
*  Function description
*    This is a key function for the display of the bitmap. It assigns
*    a clipping region to the window identical to the outline of the
*    bitmap.
*
*  Parameters
*    hWnd     : A handle to the window.
*    hBmp     : A handle to the bitmap.
*    rgbTrans : Bitmask indicating desired transparency.
*/
static void _SetBitmapRegion(HWND hWnd, HBITMAP hBmp, DWORD rgbTrans, char IsHardkey) {
  if (hBmp) {
    DWORD acBits[4096];
    HRGN  hRgn0;
    HRGN  hRgn  = 0;
    int   yOff  = 0;
    int   xOff  = 0;
    int   XSize = _GetWidthOfBitmap(hBmp);
    int   YSize = _GetYSizeOfBitmap(hBmp);
    int   y;

    if (IsHardkey) {
      RECT  WinRect;
      HWND  hWndParent = GetParent(hWnd);
      POINT Point      = { 0 };

      ClientToScreen(hWndParent, &Point);
      GetWindowRect(hWnd, &WinRect);
      YSize = WinRect.bottom - WinRect.top;
      yOff  = WinRect.top    - Point.y;
      xOff  = WinRect.left   - Point.x;
    }
    for (y = 0; y < YSize; y++) {
      int i;
      int i0;
      int i1;

      _GetScanLine(hBmp, y + yOff, &acBits[0]);
      for (i = 0; i < XSize;) {
        while (_ComparePixels(acBits[i], rgbTrans) == 0 && i < XSize) {
          i++;
        }
        i0 = i;
        while (_ComparePixels(acBits[i], rgbTrans) != 0 && i < XSize) {
          i++;
        }
        i1 = i;
        if (i0 < XSize) {
          hRgn0 = CreateRectRgn(i0 - xOff, y, i1 - xOff, y + 1);
          if (hRgn0) {
            if (hRgn) {
              CombineRgn(hRgn, hRgn, hRgn0, RGN_OR);
              DeleteObject(hRgn0);
            } else {
              hRgn = hRgn0;
            }
          }
        }
      }
    }
    SetWindowRgn(hWnd, hRgn, 0);  // Note: Region is now owned by the system, do not delete
  }
}

/*********************************************************************
*
*       _OnCommand()
*
*  Function description
*    Called when a command is sent to the main window.
*    Commands are menu commands or system commands.
*    We only handle menu commands here, since system commands (such as
*    Maximize) are handle in the default procedure
*
*  Parameters
*    hDlg   : A handle to the window procedure that received the message.
*    message: The message.
*    wParam : Additional message information. The content of this parameter depends on the value of the message parameter.
*    lParam : Additional message information. The content of this parameter depends on the value of the message parameter.
*/
static void _OnCommand(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  int wmId = LOWORD(wParam);

  //
  //  Standard Non-application specific menu items
  //
  switch (wmId) {
  case SIM_OS_IDM_ABOUT:
    DialogBox(_hInst, (LPCTSTR)SIM_OS_IDD_ABOUTBOX, hWnd, (DLGPROC)_cbAbout);
    break;
  case SIM_OS_IDM_EXIT:
    DestroyWindow(hWnd);
    return;
  }
  DefWindowProc(hWnd, Msg, wParam, lParam);
}

/*********************************************************************
*
*       _WndProc()
*
*  Function description
*    Window procedure for device.
*
*  Parameters
*    hDlg   : A handle to the window procedure that received the message.
*    message: The message.
*    wParam : Additional message information. The content of this parameter depends on the value of the message parameter.
*    lParam : Additional message information. The content of this parameter depends on the value of the message parameter.
*
*  Return value
*    Always 0.
*/
static LRESULT CALLBACK _WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  static int xPosOld;
  static int yPosOld;
  static int CaptureOn = 0;
         int xPos      = (signed short)LOWORD(lParam);  // horizontal position of cursor
         int yPos      = (signed short)HIWORD(lParam);  // vertical position of cursor

  switch (Msg) {
  case WM_CREATE:
    _SetBitmapRegion(hWnd, _hBmpDevice, _rgbTransparent, 0);
    break;
  case WM_PAINT:
  {
    HDC         hDC;
    HDC         hDCImage;
    PAINTSTRUCT ps;

    //
    // Preparations.
    //
    hDC      = BeginPaint(hWnd, &ps);
    hDCImage = CreateCompatibleDC(hDC);
    SelectObject(hDCImage, _hBmpDevice);
    //
    // Give application a chance to draw
    //
    SIM_OS_PaintWindow(hDCImage);
    //
    // Copy generated image into display
    //
    BitBlt(hDC, 0, 0, 1000, 1000, hDCImage, 0, 0, SRCCOPY);
    //
    // Cleanup
    //
    DeleteDC(hDCImage);
    EndPaint(hWnd, &ps);
    //
    // Notify the main thread after all initialization is done
    //
    if (_hInitEvent != NULL) {
      SetEvent(_hInitEvent);
    }
    break;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_COMMAND:
    _OnCommand(hWnd, Msg, wParam, lParam);
    break;
  case WM_RBUTTONDOWN:
  {
    POINT Point;

    Point.x = xPos;
    Point.y = yPos;
    ClientToScreen(hWnd, &Point);
    TrackPopupMenu(_hMenuPopup, TPM_RIGHTBUTTON, Point.x, Point.y, 0, hWnd, NULL);
    break;
  }
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    CaptureOn = 1;
    xPosOld   = xPos;
    yPosOld   = yPos;
    break;
  case WM_LBUTTONUP:
    ReleaseCapture();
    CaptureOn = 0;
    break;
  case WM_MOUSEMOVE:
    if (CaptureOn) {
      int xDiff = xPos - xPosOld;
      int yDiff = yPos - yPosOld;
      if (xDiff | yDiff) {
        RECT r;

        GetWindowRect(hWnd, &r);
        MoveWindow(hWnd, r.left + xDiff, r.top + yDiff, r.right - r.left, r.bottom - r.top, 1);
      }
    }
    break;
  default:
    return DefWindowProc(hWnd, Msg, wParam, lParam);
  }
  return 0;
}

/*********************************************************************
*
*       _RegisterClass()
*
*  Function description
*    Register window class for main window
*/
static void _RegisterClass(void) {
  WNDCLASSEX wcex = { 0 };
  HINSTANCE  hInstance;
  ATOM       Result;

  hInstance          = GetModuleHandle(NULL);
  wcex.cbSize        = sizeof(WNDCLASSEX);
  wcex.style         = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc   = _WndProc;
  wcex.lpszClassName = _sWindowClass;
  wcex.hInstance     = hInstance;
  wcex.hIcon         = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(SIM_OS_IDI_ICON), IMAGE_ICON,  0,  0, 0);
  wcex.hIconSm       = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(SIM_OS_IDI_ICON), IMAGE_ICON, 16, 16, 0);
  wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName  = NULL;
  Result             = RegisterClassEx(&wcex);
  if (Result == 0) {
    _ErrorWin32("Could not register window class");
  }
}

/*********************************************************************
*
*       _WindowThread()
*
*  Function description
*    Application main thread.
*
*  Parameters
*    lpParameter: Not used.
*
*  Return value
*    ==  0: Thread exited successfully.
*    == -1: Thread failed to create a window.
*/
static DWORD WINAPI _WindowThread(LPVOID lpParameter) {
  BITMAP    BmpDevice;
  HINSTANCE hInstance;
  HACCEL    hAcceleratorTable;
  MSG       Msg;

  OS_USEPARA(lpParameter);
  //
  // Create a window. This window is used to simulate blinking LEDs.
  //
  _RegisterClass();
  hInstance         = GetModuleHandle(NULL);
  hAcceleratorTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(SIM_OS_IDC_WINMAIN));
  _hBmpDevice       = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(SIM_OS_IDB_DEVICE), IMAGE_BITMAP, 0, 0, 0);
  _hMenuPopup       = LoadMenu(hInstance, MAKEINTRESOURCE(SIM_OS_IDC_CONTEXTMENU));
  _hMenuPopup       = GetSubMenu(_hMenuPopup, 0);
  //
  // Create main window
  //
  GetObject(_hBmpDevice, sizeof(BmpDevice), &BmpDevice);
  _hWnd = CreateWindowEx(0, _sWindowClass, "embOS Simulation",
                         WS_SYSMENU | WS_CLIPCHILDREN | WS_POPUP | WS_VISIBLE,
                         10, 20, BmpDevice.bmWidth, BmpDevice.bmHeight, NULL,
                         NULL, hInstance, NULL);
  if (_hWnd == NULL) {
    _ErrorWin32("Could not create window.");
    return -1;  // Error
  }
  //
  // Show main window
  //
  ShowWindow(_hWnd, SW_SHOWNORMAL);
#if defined(SIM_OS_TIMER_PERIOD) && (SIM_OS_TIMER_PERIOD != 0)
  //
  // Create a timer which is periodically invalidating the Win32 window in order to redraw it
  //
  SetTimer(_hWnd, 0, SIM_OS_TIMER_PERIOD, _cbTimer);
#endif
  //
  // Handle message loop
  //
  while (GetMessage(&Msg, NULL, 0, 0)) {
    if (!TranslateAccelerator(_hWnd, hAcceleratorTable, &Msg)) {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  }
  ExitProcess(0);
  return 0;
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_SIM_InitWindow()
*
*  Function description
*    Called from WinMain in order to initialize device window.
*/
int SIM_OS_InitWindow(void) {
  _hInitEvent    = CreateEvent(NULL, TRUE, FALSE, TEXT("InitEvent"));
  _hWindowThread = CreateThread(NULL, 0, _WindowThread, NULL, 0, NULL);
  //
  // Wait for the window thread to finish initialization of the device window
  //
  WaitForSingleObject(_hInitEvent, INFINITE);
  CloseHandle(_hInitEvent);
  _hInitEvent = NULL;
  return 1;  // No problem
}

/*********************************************************************
*
*       SIM_OS_PaintWindow()
*
*  Function description
*    This function is called from the callback of the main window when
*    handling the WM_PAINT message.
*    It gives the application a chance to draw the variable elements.
*    Per default, these are 8 LEDs on the PCB board.
*    This routine can be modified to show anything your application
*    requires, e.g. a graphical display (LCD).
*/
void SIM_OS_PaintWindow(void* hDC) {
  COLORREF Color;
  double   dx;
  int      i;
  int      x;
  int      y;
  int      w;
  int      h;

  //
  // Position parameters for LEDs. These depend on the image used.
  //
  x  = 213;
  y  = 46;
  w  = 10;
  h  = 20;
  dx = 17.3;
  //
  // Draw LEDs
  //
  for (i = 0; i < 8; i++) {
    Color = BSP_GetLEDState(i) ? 0x00FF80 : 0x005000;    // Select color depending on state of LED
    _DrawLED((HDC)hDC, (int)(x + i * dx), y, w, h, Color);
  }
}

/*********************************************************************
*
*       SIM_OS_UpdateWindow()
*
*  Function description
*    Immediately updates the entire device on the display.
*/
void SIM_OS_UpdateWindow(void) {
  if (_hWnd != NULL) {
    InvalidateRect(_hWnd, NULL, FALSE);
    UpdateWindow(_hWnd);
  }
}

/*************************** End of file ****************************/
