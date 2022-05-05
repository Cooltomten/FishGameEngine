#pragma once
#include "FGE/Core/Window.h"
#include "FGE/Rendering/ForwardRenderer.h"
#include "FGE/Scene/Scene.h"
#include <memory>

namespace CommonUtilities
{
	class InputManager;
	class Timer;
}
namespace FGE
{
	class Application
	{
	public:
		Application(const WindowProperties& aProperties = WindowProperties());
		~Application();

		void Run();

		inline static Application& Get() { return *myInstance; }

		inline std::shared_ptr<Window> GetWindow() { return myWindow; }
	private:
		LRESULT WndProc(HWND aHwnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam);
		
		bool myRunning;

		std::shared_ptr<Window> myWindow;

		ForwardRenderer myForwardRenderer;

		Scene* myScene;

		std::vector<std::shared_ptr<Mesh>> myModelsTest;

		std::shared_ptr<CU::InputManager> myInputManager;
		std::shared_ptr<CU::Timer> myTimer;

		
		
		static Application* myInstance;
	};
	static Application* CreateApplication();
}