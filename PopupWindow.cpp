#include "PopupWindow.h"

LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

PopupWindow::PopupWindow()
	: m_hInstance(GetModuleHandle(nullptr))
	, m_hWnd(nullptr)
	, m_info(displayinfo())
{
	const wchar_t* CLASS_NAME = L"FrameRateInfo";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WndProc;
	wndClass.hbrBackground = ::CreateSolidBrush(RGB(25, 25, 25));

	RegisterClass(&wndClass);

	//DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	DWORD style = WS_POPUPWINDOW;

	int width = 200;
	int height = 100;

	RECT rect = { 0,0,width, height };

	AdjustWindowRect(&rect, style, false);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"FPS",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	m_hdc = GetWindowDC(m_hWnd);

	ShowWindow(m_hWnd, SW_SHOW);

	m_hbmMem = CreateCompatibleBitmap(m_hdc, width, height);
	m_hdcMem = CreateCompatibleDC(m_hdc);
}

PopupWindow::~PopupWindow()
{
	DeleteObject(m_hbmMem);
	DeleteDC(m_hdcMem);

	const wchar_t* CLASS_NAME = L"FrameRateInfo";

	UnregisterClass(CLASS_NAME, m_hInstance);
}

bool PopupWindow::ProcessMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void PopupWindow::UpdateFrame(int frame, int max)
{
	float l = (cos(M_PI * (static_cast<float>(frame) / max) + M_PI) + 1) * 0.5f;
	int y = (int)lerp(0, 150, l);

	int posx = m_info.locationX + (m_info.width / 2) - 100;
	int posy = m_info.locationY + m_info.height - y;

	SetWindowPos(m_hWnd, HWND_TOP,
		posx,
		posy,
		0,
		0,
		SWP_SHOWWINDOW | SWP_NOSIZE);

	DrawFPS();
	DrawColor();
	BitBlt(m_hdc, 0, 0, 200, 100, m_hdcMem, 0, 0, SRCCOPY);
}

void PopupWindow::DrawFPS()
{	
	wchar_t buffer[256];
	wchar_t buffer_info[256];
	wsprintfW(buffer, L"[MAX]\n\n[CURRENT]");
	wsprintfW(buffer_info, L"%d\n\n%d", m_info.fps_max, m_info.fps_current);
	//wsprintfW(buffer, L"ABC");
	RECT rect_text = { 20, 20, 150, 100 };
	RECT rect_info = { 100, 20, 180, 100 };
	HFONT hFont = CreateFont(
		20,            // ��Ʈ ����
		0,              // ��Ʈ �ʺ�
		0,              // �ؽ�Ʈ ȸ�� ����
		0,              // ���� ����
		FW_NORMAL,		// ��Ʈ �β�
		FALSE,          // ���Ÿ�
		FALSE,          // ����
		FALSE,          // ��Ҽ�
		ANSI_CHARSET,   // ���� ��Ʈ
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Arial"        // ��Ʈ �̸�
	);
	HFONT hOldFont = (HFONT)SelectObject(m_hdc, hFont); // ��Ʈ ����

	SetTextColor(m_hdc, RGB(255, 255, 255));	// ���� ����
	SetBkMode(m_hdc, TRANSPARENT);			// ���� ������

	DrawText(m_hdc, buffer, -1, &rect_text, DT_LEFT | DT_VCENTER);
	DrawText(m_hdc, buffer_info, -1, &rect_info, DT_RIGHT | DT_VCENTER);

	SelectObject(m_hdc, hOldFont);
	DeleteObject(hFont);
}

void PopupWindow::DrawColor()
{
	HBRUSH hBrush{};
	if(m_info.fps_max == m_info.fps_current)
		hBrush = CreateSolidBrush(RGB(25, 230, 25));
	else
		hBrush = CreateSolidBrush(RGB(230, 25, 25));

	RECT r_bottom = { 0, 90, 210, 110 };
	FillRect(m_hdc, &r_bottom, hBrush);

	// ����� GDI ��ü ����
	DeleteObject(hBrush);
}

float PopupWindow::lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}
