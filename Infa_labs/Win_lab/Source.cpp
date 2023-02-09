#include<Windows.h>

#include <time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,

	PSTR szCmdLine, int iCmdShow)

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

	hwnd = CreateWindow(szAppName, //

		"Паравозище", //

		WS_OVERLAPPEDWINDOW, //

		CW_USEDEFAULT, //

		CW_USEDEFAULT, //

		CW_USEDEFAULT, //

		CW_USEDEFAULT, //

		NULL, //

		NULL, //

		hInstance, //

		NULL); //

	ShowWindow(hwnd, iCmdShow);

	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))

	{

		TranslateMessage(&msg);

		DispatchMessage(&msg);

	}

	return msg.wParam;

}

#define BTN_WIDTH 150

#define BTN_HEIGHT 20

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)

{

	HDC hdc;

	PAINTSTRUCT ps;

	RECT rect;

	static HWND hwndButton1, hwndButton2, hwndButton3;

	static char txt[32] = "", txt_sn[32] = "Мишачев Александр";

	int loword, hiword;

	static int fpic = 0;

	static HPEN hpen;

	static HBRUSH red, blue, black;

	switch (iMsg)

	{

	case WM_CREATE:

		srand(time(0));

		hdc = GetDC(hwnd);

		hwndButton1 = CreateWindow("button", "Паравоз",

			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,

			100, 50, BTN_WIDTH, BTN_HEIGHT,

			hwnd, (HMENU)1, hInst, NULL);

		hwndButton2 = CreateWindow("button", "Очистить рисунок",

			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,

			100, 100, BTN_WIDTH, BTN_HEIGHT,

			hwnd, (HMENU)2, hInst, NULL);

		hwndButton3 = CreateWindow("button", "Создатель",

			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,

			100, 150, BTN_WIDTH, BTN_HEIGHT,

			hwnd, (HMENU)3, hInst, NULL);

		return 0;

	case WM_COMMAND:

		hpen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));

		blue = CreateSolidBrush(RGB(0, 0, 255));

		black = CreateSolidBrush(RGB(0, 0, 0));

		red = CreateSolidBrush(RGB(255, 0, 0));

		loword = LOWORD(wParam);

		hiword = HIWORD(wParam);

		switch (loword) {

		case 1: strcpy_s(txt, "Паровоз"); fpic = 1; break;

		case 2: strcpy_s(txt, "Очистить"); fpic = 2; break;

		case 3: strcpy_s(txt, "Создатель"); fpic = 3; break;

		default: strcpy_s(txt, "Неизвестно"); break;

		}

		InvalidateRect(hwnd, NULL, TRUE);

		return 0;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);

		TextOut(hdc, 100, 300, txt, strlen(txt));

		SelectObject(hdc, hpen);

		switch (fpic) {

		case 1:

			SelectObject(hdc, black);

			Rectangle(hdc, 500, 200, 900, 400);

			SelectObject(hdc, red);

			Rectangle(hdc, 550, 150, 600, 200);

			Rectangle(hdc, 540, 125, 610, 150);

			for (int i = 0; i < 100; i++) {

				Rectangle(hdc, 500 - i, 395 + i, 500, 396 + i);

				Rectangle(hdc, 400 + abs(100 - 2 * i), 200 + 2 * i, 500, 202 + 2 * i);

				Rectangle(hdc, 750 + 3.0 / 2 * i, 50 + abs(50 - i), 752 + 3.0 / 2 * i, 200);

			}

			SelectObject(hdc, blue);

			for (int i = 0; i < 4; i++) {

				Ellipse(hdc, 800 - 100 * i, 400, 900 - 100 * i, 500);

			}

			Rectangle(hdc, 800, 110, 850, 160);

			break;

		case 2:

			break;

		case 3:

			TextOut(hdc, 700, 400, txt_sn, strlen(txt_sn));

			break;

		}

		EndPaint(hwnd, &ps);

		return 0;

	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;

	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);

}