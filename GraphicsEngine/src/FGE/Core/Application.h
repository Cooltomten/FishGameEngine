#pragma once
#include "FGE/Core/Window.h"
#include <memory>

namespace CommonUtilities
{
	class InputManager;
	class Timer;
}

namespace CU = CommonUtilities;
namespace FGE
{
	class Mesh;
	class AnimatedMesh;
	class Event;
	class ImGuiLayer;
	class Application
	{
	public:
		Application(const WindowProperties& aProperties = WindowProperties());
		~Application();

		void Run();

		void OnEvent(Event& aEvent);
		virtual void OnEventSub(Event& aEvent) {};

		inline static Application& Get() { return *myInstance; }

		inline std::shared_ptr<Window> GetWindow() { return myWindow; }

		inline std::shared_ptr<CU::InputManager> GetInput() { return myInputManager; } //TODO: fix this temporary solution
	private:
		LRESULT WndProc(HWND aHwnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam);


		bool myRunning;

		std::shared_ptr<Window> myWindow;

		std::shared_ptr<CU::InputManager> myInputManager;
		std::shared_ptr<CU::Timer> myTimer;

		static Application* myInstance;

		std::unique_ptr<FGE::ImGuiLayer> myImGuiLayer;
		
	};
	static Application* CreateApplication();
}