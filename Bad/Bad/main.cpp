#include <windows.h>

#pragma comment(lib, "msimg32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	HINSTANCE g_hInst;
	LPCTSTR lpszClass = "Bad";

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 600,
		NULL, (HMENU)NULL, hInstance, NULL);


	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HWND hStart;			// TO Make Start Message

	HDC m_hdc;
	HDC mem_hdc;		// ��¿� ����

	HBITMAP bitmap, old_bitmap;
	BITMAP bit;

	static int screen_x = GetSystemMetrics(SM_CXSCREEN);
	static int screen_y = GetSystemMetrics(SM_CYSCREEN);

	int bx;
	int by;

	switch (iMessage)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_SHOWMAXIMIZED);

	case WM_PAINT:
		m_hdc = GetDC(hWnd);
		mem_hdc = CreateCompatibleDC(m_hdc);													// parameter DC�� ������ Ư���� �޸� DC ��ȯ

		bitmap = (HBITMAP)LoadImage(NULL, "img.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);		// Load Image

		if (bitmap == NULL)																		// Image Load Fail
		{
			MessageBox(hWnd, "Failed", "Failed..", MB_OK);
			PostQuitMessage(0);
			return E_FAIL;
		}

		old_bitmap = (HBITMAP)SelectObject(mem_hdc, bitmap);									// mem_hdc �� bitmap ���� (��½� �ش� object ���)
		// return ���� ������ ���õǾ� �ִ� ���������� object �ڵ�
		
		GetObject(bitmap, sizeof(BITMAP), &bit);												// bit �� �̹��� ���� ������
		bx = bit.bmWidth;
		by = bit.bmHeight;

		TransparentBlt(m_hdc, 0, 0, screen_x, screen_y, mem_hdc, 0, 0, bx, by, RGB(255, 255, 255));				// m_hdc - screen , mem_hdc - screen buffer (we modify)
		//BitBlt(m_hdc, 0, 0, bx, by, mem_hdc, 0, 0, SRCCOPY);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);					// Program End
		break;
	default:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}