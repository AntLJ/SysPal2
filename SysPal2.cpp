// SysPal2.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "SysPal2.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SYSPAL2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYSPAL2));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

BOOL CheckDisplay(HWND hwnd)
{
	HDC hdc;
	int iPalSize;
	hdc = GetDC(hwnd);
	iPalSize = GetDeviceCaps(hdc, SIZEPALETTE);
	ReleaseDC(hwnd, hdc);
	if (iPalSize != 256)
	{
		//MessageBox(hwnd, TEXT("This program requires that the video ") TEXT("display mode have a 256-color palette."), szWindowClass, MB_ICONERROR);
		//return FALSE;
	}
	return TRUE;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSPAL2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SYSPAL2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HPALETTE hPalette;
	static int cxClient, cyClient;
	HBRUSH hBrush;
	HDC hdc;
	int i, x, y;
	LOGPALETTE * plp;
	PAINTSTRUCT ps;
	RECT rect;
	switch (message)
	{
	case WM_CREATE:
		if (!CheckDisplay(hwnd))
			return -1;
		plp = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) + 255 * sizeof(PALETTEENTRY));
		plp->palVersion = 0x0300;
		plp->palNumEntries = 256;
		for (i = 0; i < 256; i++)
		{
			plp->palPalEntry[i].peRed = i;
			plp->palPalEntry[i].peGreen = 0;
			plp->palPalEntry[i].peBlue = 0;
			plp->palPalEntry[i].peFlags = PC_EXPLICIT;
		}

		hPalette = CreatePalette(plp);
		free(plp);
		return 0;

	case WM_DISPLAYCHANGE:
		if (!CheckDisplay(hwnd))
			DestroyWindow(hwnd);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectPalette(hdc, hPalette, FALSE);
		RealizePalette(hdc);
		for (y = 0; y < 16; y++)
			for (x = 0; x < 16; x++)
			{
				hBrush = CreateSolidBrush(PALETTEINDEX(16 * y + x));
				SetRect(&rect, x * cxClient / 16, y * cyClient / 16, (x + 1) * cxClient / 16, (y + 1) * cyClient / 16);
				FillRect(hdc, &rect, hBrush);
				DeleteObject(hBrush);
			}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_PALETTECHANGED:
		if ((HWND)wParam != hwnd)
			InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		DeleteObject(hPalette);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
