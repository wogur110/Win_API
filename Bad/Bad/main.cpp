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

	hWnd = CreateWindow(lpszClass,				// mode
			    lpszClass,				// Title Name
			    WS_POPUP,				// Style
			    CW_USEDEFAULT,			// Start Position x
			    CW_USEDEFAULT,			// Start Position y
			    1000,				// Size x
			    600,				// Size y
			    NULL, (HMENU)NULL,		
			    hInstance,				// HINSTANCE
			    NULL);				// Content


	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC m_hdc;
	HDC mem_hdc;		// Buffer To Handle			( For dubble buffering )

	HBITMAP bitmap, old_bitmap;
	BITMAP bit_info;

	static int screen_x = GetSystemMetrics(SM_CXSCREEN);
	static int screen_y = GetSystemMetrics(SM_CYSCREEN);

	int image_x;
	int image_y;

	switch (iMessage)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_SHOWMAXIMIZED);

	case WM_PAINT:
		m_hdc = GetDC(hWnd);
		mem_hdc = CreateCompatibleDC(m_hdc);									// parameter DC와 동일한 특성의 메모리 DC 반환

		bitmap = (HBITMAP)LoadImage(NULL, "img.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (bitmap == NULL)											// Image Load Fail
		{
			MessageBox(hWnd, "Failed", "Failed..", MB_OK);
			PostQuitMessage(0);
			return E_FAIL;
		}

		old_bitmap = (HBITMAP)SelectObject(mem_hdc, bitmap);		// mem_hdc 에 bitmap 연결 (출력시 해당 object 사용)
										// return 값은 이전에 선택되어 있던 같은종류의 object 핸들
		
		GetObject(bitmap, sizeof(BITMAP), &bit_info);			// bit_info 에 이미지 정보 가져옴
		image_x = bit_info.bmWidth;
		image_y = bit_info.bmHeight;

		TransparentBlt(m_hdc, 0, 0, screen_x, screen_y, mem_hdc, 0, 0, image_x, image_y, RGB(255, 255, 255));	// m_hdc - dst screen , mem_hdc - src screen
		//BitBlt(m_hdc, 0, 0, image_x, image_y, mem_hdc, 0, 0, SRCCOPY);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);					// Program End
		break;
	default:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
