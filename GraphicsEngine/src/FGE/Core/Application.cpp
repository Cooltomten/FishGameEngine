#include "GraphicsEngine.pch.h"
#include "Application.h"

#include "FGE/Asset/ResourceCache.h"
#include "FGE/Asset/Material.h"
#include "FGE/Asset/AnimatedMesh.h"

#include "FGE/Rendering/Camera/Camera.h"
#include "FGE/Rendering/Renderer.h"

#include "FGE/Event/Event.h"
#include "FGE/Event/ApplicationEvents.h"

#include <iostream>

#include <CommonUtilities/InputManager.h>
#include <CommonUtilities/Timer.h>
#include <CommonUtilities/UtilityFunctions.hpp>

namespace FGE
{
	Application* Application::myInstance = nullptr;

	Application::Application(const WindowProperties& aProperties)
	{
		myInstance = this;
		myWindow = Window::Create(aProperties);
		myWindow->SetWndFunctionToCall([this](HWND aHwnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam) -> LRESULT
			{
				return WndProc(aHwnd, aMessage, aWParam, aLParam);
			});

		try
		{
			myTimer = std::make_shared<CU::Timer>();
			myInputManager = std::make_shared<CU::InputManager>();
			Renderer::Init();
			ResourceCache::Initialize();

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
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		myRunning = true;
		while (myRunning)
		{
			Window::ProcessMessages();
			
			myTimer->Update();
			myInputManager->UpdateInput();
			
			AppUpdateEvent updateEvent(myTimer->GetDeltaTime());
			OnEvent(updateEvent);
			
			myWindow->GetDX11().BeginFrame({ 0.8f,0.4f,0.2f,1 });
			AppRenderEvent renderEvent;
			OnEvent(renderEvent);
			myWindow->GetDX11().EndFrame();

		}
	}

	void Application::OnEvent(Event& aEvent)
	{
		OnEventSub(aEvent);
	}

	LRESULT FGE::Application::WndProc(HWND aHwnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam)
	{
		if (myInputManager)
		{
			myInputManager->UpdateEvents(aMessage, aWParam, aLParam);
		}

		switch (aMessage)
		{
		case WM_CLOSE:
			myRunning = false;
			break;
		}

		return 0;
	}

	

}