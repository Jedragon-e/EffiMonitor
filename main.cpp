
#include "PopupWindow.h"
#include "DisplayManager.h"

void HiddenConsole()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
}

int main()
{
	HiddenConsole();

	// Get Display Info ------------------------------
	DisplayManager* displaymanager = new DisplayManager();

	// Set Windows ------------------------------
	vector<PopupWindow*> windows;
	for (int i = 0; i < displaymanager->GetMonitorCount(); i++)
	{
		PopupWindow* popup_window = new PopupWindow();
		popup_window->SetPopupInfo(displaymanager->GetDisplayInfo(i));
		windows.push_back(popup_window);
	}

	// Play Animation ------------------------------
	int frame = 0;
	while (frame < 100)
	{
		for (auto& Pair : windows)
		{
			Pair->UpdateFrame(frame, 100);
		}
		frame += 5;
		Sleep(1);
	}
	Sleep(3000);
	while (frame > 0)
	{
		for (auto& Pair : windows)
		{
			Pair->UpdateFrame(frame, 100);
		}
		frame -= 5;
		Sleep(1);
	}
	Sleep(1000);


	// Memory delete ------------------------------
	for (PopupWindow*& Pair : windows)
	{
		delete Pair;
		Pair = nullptr;
	}
	windows.clear();

	delete displaymanager;

	return 0;
}