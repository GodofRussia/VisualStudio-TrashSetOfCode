#include <windows.h>
#define D 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "WinAP";
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wndclass);
	hwnd = CreateWindow(
		szAppName,
		"WinMain sample",
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
	//dc = GetDC(hwnd);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))// while not WM_QUIT (0) nor some error (-1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
// Message processing function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static HPEN hpen;
	static HBRUSH hbrush;
	static bool Move;
	static int Phase = 0, cxClient, cyClient, cSize = 0;
	switch (iMsg)
	{
	case WM_LBUTTONDOWN:
		Move = true;
		SetTimer(hwnd, 1, 100, NULL);
		SetTimer(hwnd, 2, 1000, NULL);
		return 0;
	case WM_RBUTTONDOWN:
		Move = false;
		KillTimer(hwnd, 1);
		InvalidateRect(hwnd, 0, TRUE);
		return 0;
	case WM_MBUTTONDOWN:
		Phase = 0;
		InvalidateRect(hwnd, 0, TRUE);
		return 0;
	case WM_TIMER:
		if (wParam == 1) {
			if (Move) {
				Phase++;
				InvalidateRect(hwnd, 0, TRUE);
			}
		}
		else if (Move) {
			cSize++;
			InvalidateRect(hwnd, 0, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hbrush = CreateSolidBrush(RGB(192, 192, 192));
		SelectObject(hdc, hbrush);
		Rectangle(hdc, cxClient / D - Phase + cSize, cyClient / D + cSize, cxClient - cxClient / D - Phase - cSize, cyClient - cyClient / D - cSize);
		hbrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hdc, hbrush);
		Rectangle(hdc, (cxClient / D) + 50 - Phase + cSize, (cyClient / D) + 50 + cSize, cxClient - (cxClient / D) - 50 - Phase - cSize, cyClient - (cyClient / D) - 50 - cSize);
		hbrush = CreateSolidBrush(RGB(255, 0, 0));
		hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		SelectObject(hdc, hbrush);
		SelectObject(hdc, hpen);
		Ellipse(hdc, (cxClient / D) + 50 - Phase + cSize, cyClient - (cyClient / D) - 40 +cSize, (cxClient / D) + 50 + 30 - Phase - cSize, cyClient - (cyClient / D) - 10 - cSize);
		hbrush = CreateSolidBrush(RGB(255, 0, 0));
		hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		SelectObject(hdc, hbrush);
		SelectObject(hdc, hpen);
		RoundRect(hdc, cxClient - (cxClient / D) - 150 - Phase + cSize, cyClient - (cyClient / D) - 40 + cSize, cxClient - (cxClient / D) - 50 - Phase - cSize, cyClient - (cyClient / D) - 10 - cSize, 25, 25);
		hbrush = CreateSolidBrush(RGB(192, 192, 192));
		hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(hdc, hpen);
		SelectObject(hdc, hbrush);
		Rectangle(hdc, (cxClient - cxClient / D) / 2 - 20 + cxClient / (D * 2) - Phase + cSize, cyClient - cyClient / D - cSize, (cxClient - cxClient / D) / 2 + 20 + cxClient / (D * 2) - Phase - cSize, (cyClient - cyClient / D) + cyClient / (D * 2) - cSize);
		Ellipse(hdc, (cxClient - cxClient / D) / 2 - 70 + cxClient / (D * 2) - Phase + cSize, cyClient - cyClient / (D * 1.5) - cSize, (cxClient - cxClient / D) / 2 + 70 + cxClient / (D * 2) - Phase - cSize, cyClient - cyClient / (D * 10) - cSize);
		DeleteObject(hpen);
		DeleteObject(hbrush);
		GetClientRect(hwnd, &rect);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
