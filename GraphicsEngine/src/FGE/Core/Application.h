#pragma once
#include "FGE/Core/Window.h"
#include "FGE/Scene/Scene.h"
#include "FGE/Transform.h"
#include <memory>

namespace CommonUtilities
{
	class InputManager;
	class Timer;
}
namespace FGE
{
	class Mesh;
	class AnimatedMesh;
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

		void CameraController();
		
		bool myRunning;

		std::shared_ptr<Window> myWindow;

		Scene* myScene;

		std::shared_ptr<Mesh> myChestMesh;
		FGE::Transform myChestTransform;
		
		std::shared_ptr<AnimatedMesh> myGremlinMesh;
		FGE::Transform myGremlinTransform;

		std::shared_ptr<CU::InputManager> myInputManager;
		std::shared_ptr<CU::Timer> myTimer;


		
		
		static Application* myInstance;
	};
	static Application* CreateApplication();
}