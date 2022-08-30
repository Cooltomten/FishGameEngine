#include "GraphicsEngine.pch.h"
#include "Window.h"
#include <sstream>
#include <string>
#include <filesystem>

namespace FGE
{
	Window::WindowClass Window::WindowClass::wndClass;

	LPCWSTR Window::WindowClass::GetName() noexcept
	{
		return wndClassName;
	}

	HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return wndClass.hInst;
	}

	Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
	{
		//Register window class
		WNDCLASSEX windowClass = { 0 };
		windowClass.cbSize = sizeof(windowClass);
		windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = HandleMsgSetup;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetInstance();
		windowClass.hIcon = nullptr;
		windowClass.hCursor = nullptr;
		windowClass.hbrBackground = nullptr;
		windowClass.lpszClassName = nullptr;
		windowClass.lpszClassName = GetName();
		windowClass.hIconSm = nullptr;
		RegisterClassEx(&windowClass);

	}

	Window::WindowClass::~WindowClass()
	{
		UnregisterClass(wndClassName, GetInstance());
	}

	Window::Window(WindowProperties aPropeties)
	{
		myWidth = aPropeties.Width;
		myHeight = aPropeties.Height;
		RECT wr;
		wr.left = aPropeties.X;
		wr.right = myWidth + wr.left;
		wr.top = aPropeties.Y;
		wr.bottom = myHeight + wr.top;

		if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		{
			throw FWND_LAST_EXCEPT();
		}

		std::filesystem::path title = aPropeties.Title;
		//create window and get hwnd
		myHWnd = CreateWindowEx(0,
			WindowClass::GetName(),
			title.generic_wstring().c_str(),
			WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
			aPropeties.X,
			aPropeties.Y,
			wr.right - wr.left,
			wr.bottom - wr.top,
			nullptr,
			nullptr,
			WindowClass::GetInstance(),
			this);


		if (myHWnd == nullptr)
		{
			throw FWND_LAST_EXCEPT();
		}
		//newly created windows start off as hidden
		ShowWindow(myHWnd, SW_SHOW);

		//Create Graphics object
		myDX11 = std::make_unique<DX11>();
		myDX11->Initialize(myHWnd);
	}


	Window::~Window()
	{
		DestroyWindow(myHWnd);
	}

	void Window::SetTitle(const std::string& aTitle)
	{
		std::filesystem::path p = aTitle;
		if (SetWindowText(myHWnd, p.generic_wstring().c_str()) == 0)
		{
			throw FWND_LAST_EXCEPT();
		}
	}

	std::optional<int> Window::ProcessMessages()
	{
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

		return {};

		return std::optional<int>();
	}

	DX11& Window::GetDX11()
	{
		if (!myDX11)
		{
			throw FWND_NOGFX_EXCEPT();
		}

		return *myDX11;
	}

	HWND Window::GetHandle()
	{
		return myHWnd;
	}

	float Window::GetWidth()
	{
		return myWidth;
	}

	float Window::GetHeight()
	{
		return myHeight;
	}

	std::shared_ptr<Window> Window::Create(const WindowProperties& aProperties)
	{
		std::shared_ptr<Window> window;
		try
		{
			window = std::make_shared<Window>(aProperties);

		}
		catch (const FabException& e)
		{
			MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		}
		catch (const std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (...)
		{
			MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);

		}
		return window;
	}

	LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);

	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (myWndFunctionToCall)
		{
			myWndFunctionToCall(hWnd, msg, wParam, lParam);
		}
		switch (msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	void Window::SetWndFunctionToCall(callback_function_wndProc aFunction)
	{
		myWndFunctionToCall = aFunction;
	}

	//Window exception stuff
	Window::HrException::HrException(int aLine, const char* aFile, HRESULT aHr) noexcept
		:Exception(aLine, aFile), myHr(aHr)
	{

	}

	const char* Window::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< GetOriginString();
		myWhatBuffer = oss.str();
		return myWhatBuffer.c_str();
	}

	const char* Window::HrException::GetType() const noexcept
	{
		return "Fab Window Exception";
	}

	std::string Window::HrException::GetErrorDescription() const noexcept
	{
		return TranslateErrorCode(myHr);
	}

	HRESULT Window::HrException::GetErrorCode() const noexcept
	{
		return myHr;
	}

	const char* Window::NoGfxException::GetType()
	{
		return nullptr;
	}

	std::string Window::Exception::TranslateErrorCode(HRESULT aHr) noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, aHr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}
		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

}