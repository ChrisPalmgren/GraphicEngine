#pragma once
#include "CWindow.h"
#include "CTimer.h"
class CApplication
{
public:
	CApplication();

	int Go();

private:
	void Frame();

	CWindow kwnd;
	CTimer timer;


};

