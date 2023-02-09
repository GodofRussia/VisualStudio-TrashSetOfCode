#include <windows.h>
#include <time.h>
#include <cstring>

#define BUTTON_W 100
#define BUTTON_H 50
#define D 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "HelloWin";
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName, 
		"Размер рабочей области", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	); 

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static int cxClient, cyClient, fpic;
	static HPEN hpen;
	static HBRUSH hbrush;
	static int loword;
	char txt[20] = "Мочалов";
	switch (iMsg)
	{
	case WM_CREATE:
		srand(time(0));
		hdc = GetDC(hwnd);
		HWND hwndButton1, hwndButton2, hwndButton3;
		hwndButton1 = CreateWindow("button", "SHOW",
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			0, 0, BUTTON_W, BUTTON_H,
			hwnd, (HMENU)1, NULL, NULL
		);
		hwndButton2 = CreateWindow("button", "CLEAR",
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			100, 0, BUTTON_W, BUTTON_H,
			hwnd, (HMENU)2, NULL, NULL
		);
		hwndButton3 = CreateWindow("button", "P.S.",
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			200, 0, BUTTON_W, BUTTON_H,
			hwnd, (HMENU)3, NULL, NULL
		);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_COMMAND:
		loword = LOWORD(wParam);
		switch (loword)
		{
		case 1:
			fpic = 1;
			break;
		case 2:
			fpic = 2;
			break;
		case 3:
			fpic = 3;
			break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		switch (fpic) {
		case 1:
			hbrush = CreateSolidBrush(RGB(192, 192, 192));
			SelectObject(hdc, hbrush);
			Rectangle(hdc, cxClient/D, cyClient/D, cxClient - cxClient / D, cyClient - cyClient / D);
			hbrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hdc, hbrush);
			Rectangle(hdc, (cxClient / D) + 50, (cyClient / D) + 50, cxClient - (cxClient / D) - 50, cyClient - (cyClient / D) - 50);
			hbrush = CreateSolidBrush(RGB(255, 0, 0));
			hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			SelectObject(hdc, hbrush);
			SelectObject(hdc, hpen);
			Ellipse(hdc, (cxClient / D) + 50, cyClient - (cyClient / D) - 40, (cxClient / D) + 50 + 30, cyClient - (cyClient / D) - 10);
			hbrush = CreateSolidBrush(RGB(255, 0, 0));
			hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			SelectObject(hdc, hbrush);
			SelectObject(hdc, hpen);
			RoundRect(hdc, cxClient - (cxClient / D) - 150, cyClient - (cyClient / D) - 40, cxClient - (cxClient / D) - 50, cyClient - (cyClient / D) - 10, 25, 25);
			hbrush = CreateSolidBrush(RGB(192, 192, 192));
			hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, hpen);
			SelectObject(hdc, hbrush);
			Rectangle(hdc, (cxClient - cxClient / D)/2 - 20 + cxClient/(D*2), cyClient - cyClient / D, (cxClient - cxClient / D) / 2 + 20 + cxClient / (D*2), (cyClient - cyClient / D) + cyClient/(D*2));
			Ellipse(hdc, (cxClient - cxClient / D) / 2 - 70 + cxClient / (D*2), cyClient - cyClient / (D*1.5), (cxClient - cxClient / D) / 2 + 70 + cxClient / (D*2), cyClient - cyClient / (D * 10));
			break;
		case 2:
			hbrush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, hbrush);
			Rectangle(hdc, 0, 0, cxClient, cyClient);
			break;
		case 3:
			TextOut(hdc, cxClient / 2, cyClient / 2, txt, strlen(txt));
			break;
		}

		GetClientRect(hwnd, &rect);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
