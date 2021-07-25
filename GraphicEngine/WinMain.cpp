#include "CApplication.h"

#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

// TODO: Fix this
// Converter to translate utf-8 to utf-16
// upd: Conversion works better now, just desc now that gets misinterpreted
// Need to take a closer look at this so I can pass in correct byte size from the start withouth converting.
// upd: If I replace my Exceptional manager with something else, I might not need this converter (should save this though and build on it further.)
wchar_t* convert816(const char* src)
{
	size_t size = strlen(src) + 1;
	wchar_t* w_dest = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, w_dest, size, src, size - 1);
	return w_dest;
}

int CALLBACK WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow)
{
	try
	{
		return CApplication{}.Go();
	}
	catch (const CException& e)
	{
		MessageBox(
			nullptr,
			convert816(e.what()),
			convert816(e.GetType()),
			MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(
			nullptr,
			convert816(e.what()),
			L"Standard Exception",
			MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(
			nullptr,
			L"No Details Available",
			L"Unknown Exception",
			MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}