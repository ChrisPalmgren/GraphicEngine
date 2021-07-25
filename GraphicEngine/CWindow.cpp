#include "CWindow.h"
#include <sstream>
#include "resource.h"

CWindow::CWindowClass CWindow::CWindowClass::kwndClass;

LPCWSTR CWindow::CWindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE CWindow::CWindowClass::GetInstance() noexcept
{
	return kwndClass.hInst;
}

CWindow::CWindowClass::CWindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage( hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 128, 128, 0));
	wc.hCursor = 0;
	wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 0, 0)));
	wc.lpszMenuName = 0;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0));
	RegisterClassEx(&wc);
}

CWindow::CWindowClass::~CWindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

CWindow::CWindow(int width, int height, LPCWSTR name)
	: width(width), height(height)
{
	// Calculate size based on desired client region size.
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, K_W_STYLE, FALSE) == 0)
	{
		throw KEXCEPT_LAST_EXCEPT();
	}

	// Create
	hWnd = CreateWindow(
		CWindowClass::GetName(), name,
		K_W_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		0, 0, CWindowClass::GetInstance(), this // "this" to refer lparam to this instance while managing messages.
	);

	if (hWnd == nullptr)
	{
		throw(KEXCEPT_LAST_EXCEPT());
	}

	// Show
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

CWindow::~CWindow()
{
	DestroyWindow(hWnd);
}
void CWindow::SetTitle(const std::wstring& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw KEXCEPT_LAST_EXCEPT();
	}
}
std::optional<int> CWindow::ProcessMessages()
{
	// Message filtering
	// Looks for specific messages in thread message queue and returns of they exist.
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return{};
}

LRESULT CWindow::HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (uMsg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		CWindow* const pWnd = static_cast<CWindow*>(pCreate->lpCreateParams);

		// MSDN: Changes an attribute of the specified window. The function also sets a value at the specified offset in the extra window memory.
		//			Note: To write code that is compatible with both 32-bit and 64-bit versions of Windows, use SetWindowLongPtr. 
		//			When compiling for 32-bit Windows, SetWindowLongPtr is defined as a call to the SetWindowLong function.
		//			https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra
		//
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWindow::HandleMsgThunk));

		return pWnd->HandleMsg(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CWindow::HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	CWindow* const pWnd = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, uMsg, wParam, lParam);
}

LRESULT CWindow::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (uMsg)
	{
	// On close of window
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	// Keyboard Events
	case WM_KEYDOWN: case WM_SYSKEYDOWN:
	{
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	}
	case WM_KEYUP: case WM_SYSKEYUP:
	{
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_CHAR:
	{
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	}
	// Mouse Events
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		if ((pt.x >= 0 && pt.x < width) && (pt.y >= 0 && pt.y < height))
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.onMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	// On killed focus of this window
	case WM_KILLFOCUS:
	{
		kbd.ClearState();
		break;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CWindow::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	: CException(line, file), hr(hr)
{
}

const char* CWindow::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl <<
		"[Error Code] " << GetErrorCode() << std::endl <<
		"[Description]" << GetErrorString() << std::endl <<
		GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* CWindow::Exception::GetType() const noexcept
{
	return "Krispy Exception";
}

std::string CWindow::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage
	(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0)
		return "Unidentified Error Code";

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT CWindow::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string CWindow::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode( hr );
}
