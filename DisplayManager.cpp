#include "DisplayManager.h"

DisplayManager::DisplayManager()
{
	DISPLAY_DEVICE G_device;
	ZeroMemory(&G_device, sizeof(G_device));
	G_device.cb = sizeof(DISPLAY_DEVICE);
	DWORD deviceNum = 0;

	while (EnumDisplayDevices(NULL, deviceNum, &G_device, 0)) 
	{
		DEVMODE devmode{};
		devmode.dmSize = sizeof(DEVMODE);
		if (EnumDisplaySettings(G_device.DeviceName, ENUM_CURRENT_SETTINGS, &devmode))
		{
			MonitorInfo info{};
			info.ID = deviceNum + 1;
			info.bIsPrimary = (G_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) > 0;
			info.CurrentMode = devmode;

			MonitorList.push_back(info);
		}


		// 모니터의 변경 가능한 정보 리스트 저장
		DEVMODE devmode_List{};
		devmode_List.dmSize = sizeof(DEVMODE);
		DWORD modeNum = 0;
		while (EnumDisplaySettings(G_device.DeviceName, modeNum, &devmode_List)) {
			MonitorList[deviceNum].ResolutionList.push_back(devmode_List);
			modeNum++;
		}
		deviceNum++;
	}

}

DisplayManager::~DisplayManager()
{
}

displayinfo DisplayManager::GetDisplayInfo(int index)
{
	displayinfo temp{};
	temp.locationX = MonitorList[index].CurrentMode.dmPosition.x;
	temp.locationY = MonitorList[index].CurrentMode.dmPosition.y;
	temp.width = MonitorList[index].CurrentMode.dmPelsWidth;
	temp.height = MonitorList[index].CurrentMode.dmPelsHeight;
	temp.fps_current = GetCurrent(index);
	temp.fps_max = GetMax(index);

	return temp;
}

int DisplayManager::GetCurrent(int index)
{
	return MonitorList[index].CurrentMode.dmDisplayFrequency;
}

int DisplayManager::GetMax(int index)
{
	int result = 30;
	for (DEVMODE& Pair : MonitorList[index].ResolutionList)
	{
		if (result < Pair.dmDisplayFrequency)
			result = Pair.dmDisplayFrequency;
	}

	return result;
}

int DisplayManager::GetMonitorCount()
{
	return MonitorList.size();
}
