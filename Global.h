#pragma once

#ifndef SOME_HEADER_H
#define SOME_HEADER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>

#include <Windows.h>

using namespace std;

struct displayinfo
{
	int locationX = 0;
	int locationY = 0;
	int width = 10;
	int height = 10;

	int fps_max = 60;
	int fps_current = 30;
};

#endif // SOME_HEADER_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
