#pragma once
#include "CWin.h"
#include "CException.h"
#include "CKeyboard.h"
#include "CMouse.h"
#include <optional>

#define K_W_STYLE (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
#define KEXCEPT(hr) CWindow::Exception(__LINE__,__FILE__, hr)
#define KEXCEPT_LAST_EXCEPT() CWindow::Exception(__LINE__,__FILE__,GetLastError())

// Creates window
class CWindow
{
public:
	// Child to kException to manage windows specific error reports.
	// need to read on more on this - PTODO
	class Exception : public CException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	// Registers and unregisters windowclass
	class CWindowClass
	{
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		CWindowClass() noexcept;
		~CWindowClass();
		CWindowClass(const CWindowClass&) = delete;
		CWindowClass& operator= (const CWindowClass&) = delete;

		static constexpr LPCWSTR wndClassName = L"ClassName";
		static CWindowClass kwndClass;
		HINSTANCE hInst;
	};
public:
	CWindow(int width, int height, LPCWSTR name);
	~CWindow();
	CWindow(const CWindow&) = delete;
	CWindow& operator= (const CWindow&) = delete;

	void SetTitle(const std::wstring& title);
	static std::optional<int> ProcessMessages();

	CKeyboard kbd;
	CMouse mouse;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	int width;
	int height;
	HWND hWnd;
};