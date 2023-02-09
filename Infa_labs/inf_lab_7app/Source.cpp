#include <windows.h>
#define D 5
#define DIVISIONS 2

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
	static BOOL f[DIVISIONS][DIVISIONS] = { 0,0,0,0 };
	static HPEN hpen;
	static HBRUSH hbrush;
	static bool Move1, Move2;
	static int Phasex = 0, Phasey = 0, cxClient, cyClient, cx = 0, cy = 0, cxBlock = 0, cyBlock = 0, x = 0, y = 0, fic;
	switch (iMsg)
	{
	case WM_LBUTTONDOWN:
		Move1 = true;
		Move2 = true;
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		x = LOWORD(lParam) / cxBlock;
		y = LOWORD(lParam) / cyBlock;
		Phasex = 0;
		Phasey = 0;
		f[x][y] = 1;
		SetTimer(hwnd, 1, 100, NULL);
		return 0;
	case WM_RBUTTONDOWN:
		Move1 = false;
		Move2 = false;
		KillTimer(hwnd, 1);
		InvalidateRect(hwnd, 0, TRUE);
		return 0;
	case WM_MBUTTONDOWN:
		Phasex = 0;
		Phasey = 0;
		InvalidateRect(hwnd, 0, TRUE);
		return 0;
	case WM_TIMER:
		if (Move1 || Move2) {
			InvalidateRect(hwnd, 0, TRUE);
		}
		else {
			KillTimer(hwnd, 1);
			InvalidateRect(hwnd, 0, TRUE);
		}
		return 0;
	case WM_PAINT:
		for (int i = 0; i <DIVISIONS; i++)
			for (int j = 0; j < DIVISIONS; j++)
				if (i == 0 && j == 0 && f[i][j]) 
				{
					if (Phasex > (cxClient / 2 - cx)) 
					{
						Move1 = false;
					}
					if (Phasey > (cyClient / 2 - cy))
					{
						Move2 = false;
					}
					if (Move1) Phasex++;
					if (Move2) Phasey++;
					hdc = BeginPaint(hwnd, &ps);
					hbrush = CreateSolidBrush(RGB(192, 192, 192));
					SelectObject(hdc, hbrush);
					Rectangle(hdc, cxClient / D - Phasex, cyClient / D - Phasey, cxClient - cxClient / D - Phasex, cyClient - cyClient / D - Phasey);
					hbrush = CreateSolidBrush(RGB(0, 0, 0));
					SelectObject(hdc, hbrush);
					Rectangle(hdc, (cxClient / D) + 50 - Phasex, (cyClient / D) + 50 - Phasey, cxClient - (cxClient / D) - 50 - Phasex, cyClient - (cyClient / D) - 50 - Phasey);
					hbrush = CreateSolidBrush(RGB(255, 0, 0));
					hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
					SelectObject(hdc, hbrush);
					SelectObject(hdc, hpen);
					Ellipse(hdc, (cxClient / D) + 50 - Phasex, cyClient - (cyClient / D) - 40 - Phasey, (cxClient / D) + 50 + 30 - Phasex, cyClient - (cyClient / D) - 10 - Phasey);
					hbrush = CreateSolidBrush(RGB(255, 0, 0));
					hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
					SelectObject(hdc, hbrush);
					SelectObject(hdc, hpen);
					RoundRect(hdc, cxClient - (cxClient / D) - 150 - Phasex, cyClient - (cyClient / D) - 40 - Phasey, cxClient - (cxClient / D) - 50 - Phasex, cyClient - (cyClient / D) - 10 - Phasey, 25, 25);
					hbrush = CreateSolidBrush(RGB(192, 192, 192));
					hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
					SelectObject(hdc, hpen);
					SelectObject(hdc, hbrush);
					Rectangle(hdc, (cxClient - cxClient / D) / 2 - 20 + cxClient / (D * 2) - Phasex, cyClient - cyClient / D - Phasey, (cxClient - cxClient / D) / 2 + 20 + cxClient / (D * 2) - Phasex, (cyClient - cyClient / D) + cyClient / (D * 2) - Phasey);
					Ellipse(hdc, (cxClient - cxClient / D) / 2 - 70 + cxClient / (D * 2) - Phasex, cyClient - cyClient / (D * 1.5) - Phasey, (cxClient - cxClient / D) / 2 + 70 + cxClient / (D * 2) - Phasex, cyClient - cyClient / (D * 10) - Phasey);
					DeleteObject(hpen);
					DeleteObject(hbrush);
					GetClientRect(hwnd, &rect);
					EndPaint(hwnd, &ps);
				}
		else if (i != 0 && j == 0 && f[i][j])
		{
			if (Phasex > (cx - cxClient / 2))
			{
				Move1 = false;
			}
			if (Phasey > (cyClient / 2 - cy))
			{
				Move2 = false;
			}
			if (Move1) Phasex++;
			if (Move2) Phasey++;
			hdc = BeginPaint(hwnd, &ps);
			hbrush = CreateSolidBrush(RGB(192, 192, 192));
			SelectObject(hdc, hbrush);
			Rectangle(hdc, cxClient / D + Phasex, cyClient / D - Phasey, cxClient - cxClient / D + Phasex, cyClient - cyClient / D - Phasey);
			hbrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hdc, hbrush);
			Rectangle(hdc, (cxClient / D) + 50 + Phasex, (cyClient / D) + 50 - Phasey, cxClient - (cxClient / D) - 50 + Phasex, cyClient - (cyClient / D) - 50 - Phasey);
			hbrush = CreateSolidBrush(RGB(255, 0, 0));
			hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			SelectObject(hdc, hbrush);
			SelectObject(hdc, hpen);
			Ellipse(hdc, (cxClient / D) + 50 + Phasex, cyClient - (cyClient / D) - 40 - Phasey, (cxClient / D) + 50 + 30 + Phasex, cyClient - (cyClient / D) - 10 - Phasey);
			hbrush = CreateSolidBrush(RGB(255, 0, 0));
			hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			SelectObject(hdc, hbrush);
			SelectObject(hdc, hpen);
			RoundRect(hdc, cxClient - (cxClient / D) - 150 + Phasex, cyClient - (cyClient / D) - 40 - Phasey, cxClient - (cxClient / D) - 50 + Phasex, cyClient - (cyClient / D) - 10 - Phasey, 25, 25);
			hbrush = CreateSolidBrush(RGB(192, 192, 192));
			hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, hpen);
			SelectObject(hdc, hbrush);
			Rectangle(hdc, (cxClient - cxClient / D) / 2 - 20 + cxClient / (D * 2) + Phasex, cyClient - cyClient / D - Phasey, (cxClient - cxClient / D) / 2 + 20 + cxClient / (D * 2) + Phasex, (cyClient - cyClient / D) + cyClient / (D * 2) - Phasey);
			Ellipse(hdc, (cxClient - cxClient / D) / 2 - 70 + cxClient / (D * 2) + Phasex, cyClient - cyClient / (D * 1.5) - Phasey, (cxClient - cxClient / D) / 2 + 70 + cxClient / (D * 2) + Phasex, cyClient - cyClient / (D * 10) - Phasey);
			DeleteObject(hpen);
			DeleteObject(hbrush);
			GetClientRect(hwnd, &rect);
			EndPaint(hwnd, &ps);
		}
		else if (i == 0 && j != 0 && f[i][j])
				{
					if (Phasex > (cxClient / 2 - cx))
					{
						Move1 = false;
					}
					if (Phasey > (cy - cyClient / 2 ))
					{
						Move2 = false;
					}
					if (Move1) Phasex++;
					if (Move2) Phasey++;
					hdc = BeginPaint(hwnd, &ps);
					hbrush = CreateSolidBrush(RGB(192, 192, 192));
					SelectObject(hdc, hbrush);
					Rectangle(hdc, cxClient / D - Phasex, cyClient / D + Phasey, cxClient - cxClient / D - Phasex, cyClient - cyClient / D + Phasey);
					hbrush = CreateSolidBrush(RGB(0, 0, 0));
					SelectObject(hdc, hbrush);
					Rectangle(hdc, (cxClient / D) + 50 - Phasex, (cyClient / D) + 50 + Phasey, cxClient - (cxClient / D) - 50 - Phasex, cyClient - (cyClient / D) - 50 + Phasey);
					hbrush = CreateSolidBrush(RGB(255, 0, 0));
					hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
					SelectObject(hdc, hbrush);
					SelectObject(hdc, hpen);
					Ellipse(hdc, (cxClient / D) + 50 - Phasex, cyClient - (cyClient / D) - 40 + Phasey, (cxClient / D) + 50 + 30 - Phasex, cyClient - (cyClient / D) - 10 + Phasey);
					hbrush = CreateSolidBrush(RGB(255, 0, 0));
					hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
					SelectObject(hdc, hbrush);
					SelectObject(hdc, hpen);
					RoundRect(hdc, cxClient - (cxClient / D) - 150 - Phasex, cyClient - (cyClient / D) - 40 + Phasey, cxClient - (cxClient / D) - 50 - Phasex, cyClient - (cyClient / D) - 10 + Phasey, 25, 25);
					hbrush = CreateSolidBrush(RGB(192, 192, 192));
					hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
					SelectObject(hdc, hpen);
					SelectObject(hdc, hbrush);
					Rectangle(hdc, (cxClient - cxClient / D) / 2 - 20 + cxClient / (D * 2) - Phasex, cyClient - cyClient / D + Phasey, (cxClient - cxClient / D) / 2 + 20 + cxClient / (D * 2) - Phasex, (cyClient - cyClient / D) + cyClient / (D * 2) + Phasey);
					Ellipse(hdc, (cxClient - cxClient / D) / 2 - 70 + cxClient / (D * 2) - Phasex, cyClient - cyClient / (D * 1.5) + Phasey, (cxClient - cxClient / D) / 2 + 70 + cxClient / (D * 2) - Phasex, cyClient - cyClient / (D * 10) + Phasey);
					DeleteObject(hpen);
					DeleteObject(hbrush);
					GetClientRect(hwnd, &rect);
					EndPaint(hwnd, &ps);
				}
		else if (i != 0 && j != 0 && f[i][j])
				{
				if (Phasex > (cx - cxClient / 2)){
					Move1 = false;
				}
				if (Phasey > (cy - cyClient / 2))
				{
					Move2 = false;
				}
				if (Move1) Phasex++;
				if (Move2) Phasey++;
				hdc = BeginPaint(hwnd, &ps);
				hbrush = CreateSolidBrush(RGB(192, 192, 192));
				SelectObject(hdc, hbrush);
				Rectangle(hdc, cxClient / D + Phasex, cyClient / D + Phasey, cxClient - cxClient / D + Phasex, cyClient - cyClient / D + Phasey);
				hbrush = CreateSolidBrush(RGB(0, 0, 0));
				SelectObject(hdc, hbrush);
				Rectangle(hdc, (cxClient / D) + 50 + Phasex, (cyClient / D) + 50 + Phasey, cxClient - (cxClient / D) - 50 + Phasex, cyClient - (cyClient / D) - 50 + Phasey);
				hbrush = CreateSolidBrush(RGB(255, 0, 0));
				hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
				SelectObject(hdc, hbrush);
				SelectObject(hdc, hpen);
				Ellipse(hdc, (cxClient / D) + 50 + Phasex, cyClient - (cyClient / D) - 40 + Phasey, (cxClient / D) + 50 + 30 + Phasex, cyClient - (cyClient / D) - 10 + Phasey);
				hbrush = CreateSolidBrush(RGB(255, 0, 0));
				hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
				SelectObject(hdc, hbrush);
				SelectObject(hdc, hpen);
				RoundRect(hdc, cxClient - (cxClient / D) - 150 + Phasex, cyClient - (cyClient / D) - 40 + Phasey, cxClient - (cxClient / D) - 50 + Phasex, cyClient - (cyClient / D) - 10 + Phasey, 25, 25);
				hbrush = CreateSolidBrush(RGB(192, 192, 192));
				hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				SelectObject(hdc, hpen);
				SelectObject(hdc, hbrush);
				Rectangle(hdc, (cxClient - cxClient / D) / 2 - 20 + cxClient / (D * 2) + Phasex, cyClient - cyClient / D + Phasey, (cxClient - cxClient / D) / 2 + 20 + cxClient / (D * 2) + Phasex, (cyClient - cyClient / D) + cyClient / (D * 2) + Phasey);
				Ellipse(hdc, (cxClient - cxClient / D) / 2 - 70 + cxClient / (D * 2) + Phasex, cyClient - cyClient / (D * 1.5) + Phasey, (cxClient - cxClient / D) / 2 + 70 + cxClient / (D * 2) + Phasex, cyClient - cyClient / (D * 10) + Phasey);
				DeleteObject(hpen);
				DeleteObject(hbrush);
				GetClientRect(hwnd, &rect);
				EndPaint(hwnd, &ps);
				}
		else 
{

					hdc = BeginPaint(hwnd, &ps);
					hbrush = CreateSolidBrush(RGB(192, 192, 192));
					SelectObject(hdc, hbrush);
					Rectangle(hdc, cxClient / D - Phasex, cyClient / D, cxClient - cxClient / D , cyClient - cyClient / D);
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
					RoundRect(hdc, cxClient - (cxClient / D) - 150 , cyClient - (cyClient / D) - 40, cxClient - (cxClient / D) - 50 , cyClient - (cyClient / D) - 10, 25, 25);
					hbrush = CreateSolidBrush(RGB(192, 192, 192));
					hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
					SelectObject(hdc, hpen);
					SelectObject(hdc, hbrush);
					Rectangle(hdc, (cxClient - cxClient / D) / 2 - 20 + cxClient / (D * 2), cyClient - cyClient / D, (cxClient - cxClient / D) / 2 + 20 + cxClient / (D * 2) , (cyClient - cyClient / D) + cyClient / (D * 2));
					Ellipse(hdc, (cxClient - cxClient / D) / 2 - 70 + cxClient / (D * 2) , cyClient - cyClient / (D * 1.5), (cxClient - cxClient / D) / 2 + 70 + cxClient / (D * 2) , cyClient - cyClient / (D * 10));
					DeleteObject(hpen);
					DeleteObject(hbrush);
					GetClientRect(hwnd, &rect);
					EndPaint(hwnd, &ps);
				}
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxBlock = cxClient / DIVISIONS;
		cyBlock = cyClient / DIVISIONS;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
