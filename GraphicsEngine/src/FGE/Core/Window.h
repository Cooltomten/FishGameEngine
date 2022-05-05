#pragma once
#include "FabWin.h"
#include "FabException.h"
#include "DX11.h" 
#include <optional>
#include <memory>
#include <functional>



namespace FGE
{
	enum class WindowMode
	{
		Fullscreen,
		Windowed,
		Borderless
	};
	using callback_function_wndProc = std::function<LRESULT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>;
	struct WindowProperties
	{
		WindowProperties(const std::string& aTitle = "DX11", uint32_t aWidth = 1280, uint32_t aHeight = 720, bool aEnableVSync = true, WindowMode aWindowMode = WindowMode::Windowed)
			: Title(aTitle), Width(aWidth), Height(aHeight), IsVSync(aEnableVSync), WindowMode(aWindowMode)
		{}
		
		std::string Title;
		uint32_t X;
		uint32_t Y;
		uint32_t Width;
		uint32_t Height;
		bool IsVSync;
		WindowMode WindowMode;
	};
	class Window
	{
	public:
		class Exception : public FabException
		{
			using FabException::FabException;
		public:
			static std::string TranslateErrorCode(HRESULT aHr) noexcept;
		};


		class HrException : public Exception
		{
		public:
			HrException(int aLine, const char* aFile, HRESULT aHr) noexcept;
			const char* what()const noexcept override;
			virtual const char* GetType() const noexcept override;

			std::string GetErrorDescription() const noexcept;
			HRESULT GetErrorCode() const noexcept;
		private:
			HRESULT myHr;
		};
		class NoGfxException : public Exception
		{
		public:
			using Exception::Exception;
			const char* GetType();
		};
	private:
		class WindowClass
		{
		public:
			static LPCWSTR GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator=(const WindowClass&) = delete;
			static constexpr LPCWSTR wndClassName = L"Fabian Direct 3D Engine Window";
			static WindowClass wndClass;
			HINSTANCE hInst;
		};

	public:
		Window(WindowProperties aPropeties);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		void SetTitle(const std::string& aTitle);
		static std::optional<int> ProcessMessages();
		DX11& GetDX11();
		HWND GetHandle();
		
		void SetWndFunctionToCall(callback_function_wndProc aFunction);

	public:
		static std::shared_ptr<Window> Create(const WindowProperties& aProperties);

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		int myWidth;
		int myHeight;
		HWND myHWnd;
		std::unique_ptr<DX11> myDX11;
		callback_function_wndProc myWndFunctionToCall;
	};
}

#define FWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define FWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define FWND_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)

