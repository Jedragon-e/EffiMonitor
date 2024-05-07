#pragma once

#include "Global.h"

class DisplayManager
{

public:
	DisplayManager();
	~DisplayManager();

	displayinfo GetDisplayInfo(int index);
	int GetCurrent(int index);
	int GetMax(int index);
	int GetMonitorCount();

private:
	struct MonitorInfo
	{
		int ID;
		bool bIsPrimary;
		DEVMODE CurrentMode;
		vector<DEVMODE> ResolutionList;
	};

private:
	vector<MonitorInfo> MonitorList;
};

