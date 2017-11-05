#include <Windows.h>
#include <tchar.h>
#include <ctime>

#include "Misc\Settings.h"
#include "GameMain.h"

int screenKeys[512];
static HWND screenHandle = NULL;		// 主窗口 HWND
static HDC screenDC = NULL;			// 配套的 HDC
static HBITMAP screenHB = NULL;		// DIB
static HBITMAP screenOB = NULL;		// 老的 BITMAP
unsigned char *frameBuffer = NULL;	

static LRESULT ScreenEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CLOSE: break;
		case WM_KEYDOWN: break;
		case WM_KEYUP: break;
		default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void ScreenDispatch() {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void ScreenUpdate() {
	HDC hDC = GetDC(screenHandle);
	BitBlt(hDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, screenDC, 0, 0, SRCCOPY);
	ReleaseDC(screenHandle, hDC);
	ScreenDispatch();
}

int CloseScreen() {
	if (screenDC) {
		if (screenOB) {
			SelectObject(screenDC, screenOB);
			screenOB = NULL;
		}
		DeleteDC(screenDC);
		screenDC = NULL;
	}
	if (screenHB) {
		DeleteObject(screenHB);
		screenHB = NULL;
	}
	if (screenHandle) {
		CloseWindow(screenHandle);
		screenHandle = NULL;
	}
	return 0;
}

int InitScreen(int w, int h, const TCHAR *title)
{
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)ScreenEvents, 0, 0, 0,
		NULL, NULL, NULL, NULL, _T("Test") };
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
		w * h * 4, 0, 0, 0, 0 } };

	RECT rect = { 0, 0, w, h };
	int wx, wy, sx, sy;
	LPVOID ptr;
	HDC hDC;

	CloseScreen();

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if (!RegisterClass(&wc)) return -1;

	screenHandle = CreateWindow(_T("Test"), title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (screenHandle == NULL) return -2;

	hDC = GetDC(screenHandle);
	screenDC = CreateCompatibleDC(hDC);
	ReleaseDC(screenHandle, hDC);

	screenHB = CreateDIBSection(screenDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (screenHB == NULL) return -3;

	screenOB = (HBITMAP)SelectObject(screenDC, screenHB);
	frameBuffer = (unsigned char*)ptr;

	AdjustWindowRect(&rect, GetWindowLong(screenHandle, GWL_STYLE), 0);
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(screenHandle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(screenHandle);

	ShowWindow(screenHandle, SW_NORMAL);
	ScreenDispatch();

	memset(screenKeys, 0, sizeof(int) * 512);
	memset(frameBuffer, 0, w * h * 4);

	return 0;
}

void DrawPixel(int x, int y, float r, float g, float b)
{
	int index = (x + y * SCREEN_WIDTH) * 4;
	frameBuffer[index + 0] = b * 255;
	frameBuffer[index + 1] = g * 255;
	frameBuffer[index + 2] = r * 255;
	//frameBuffer[index + 3] = b * 255;
}

int main()
{
	TCHAR *title = _T("Software Renderer");
	int initResult = InitScreen(SCREEN_WIDTH, SCREEN_HEIGHT, title);
	Init(DrawPixel);

	time_t previous = clock();
	while (initResult == 0 && screenKeys[VK_ESCAPE] == 0) {
		ScreenUpdate();
		Update();

		time_t now = clock();
		float fps = 1.0f / (((float)now - (float)previous) / CLOCKS_PER_SEC);
		previous = now;
		//cout << fps << endl;
	}

	return 0;
}