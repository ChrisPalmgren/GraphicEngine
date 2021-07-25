#include "CApplication.h"

CApplication::CApplication() : kwnd(640, 480, L"Engine")
{}

int CApplication::Go()
{
	while (true)
	{
		// If message been found and recieved we return with it.
		// else we continue presenting our frame.
		if (const auto ecode = CWindow::ProcessMessages())
		{
			return *ecode;
		}
		Frame();
	}
}

void CApplication::Frame()
{
	const float t = timer.Peek();
	std::wstringstream wss;
	wss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	kwnd.SetTitle(wss.str());
}
