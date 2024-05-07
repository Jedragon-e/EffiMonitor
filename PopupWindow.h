#pragma once

#include "Global.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class PopupWindow
{
public:
	PopupWindow();
	PopupWindow(const PopupWindow&) = delete;
	~PopupWindow();
	PopupWindow& operator =(const PopupWindow&) = delete;
	bool ProcessMessages();

public:
	void UpdateFrame(int frame, int max);
	void SetPopupInfo(displayinfo info) { m_info = info; }

	void DrawFPS();
	void DrawColor();

	float lerp(float a, float b, float t);

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HDC m_hdc;

	HBITMAP m_hbmMem;
	HDC m_hdcMem;

	displayinfo m_info;
};

