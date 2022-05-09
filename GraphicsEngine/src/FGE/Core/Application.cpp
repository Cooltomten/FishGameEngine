#include "GraphicsEngine.pch.h"
#include "Application.h"
#include "FGE/Asset/ResourceCache.h"

#include "FGE/Rendering/Camera/Camera.h"
#include "FGE/Rendering/Renderer.h"

#include <iostream>
#include <CommonUtilities/InputManager.h>
#include <CommonUtilities/Timer.h>

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
			//myForwardRenderer.Initialize();
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


		myScene = new Scene();
		std::shared_ptr<Camera> camera = std::make_shared<Camera>();
		camera->SetPerspectiveProjection(90, { 1280,720 }, 0.1f, 1000.0f);
		camera->GetTransform().SetPosition({0,0,-200});
		myScene->SetMainCamera(camera);
		
		myModelsTest.push_back(ResourceCache::GetAsset("Cube"));
	}

	Application::~Application()
	{
		delete myScene;
	}

	void Application::Run()
	{
		myRunning = true;
		while (myRunning)
		{
			Window::ProcessMessages();
			
			myTimer->Update();
			myInputManager->UpdateInput();

			std::cout << myInputManager->GetMousePosDelta().x << " " << myInputManager->GetMousePosDelta().y << std::endl;


			myTestTransform.SetRotation(myTestTransform.GetRotation().x + myTimer->GetDeltaTime() * 0.5f,
				myTestTransform.GetRotation().y + myTimer->GetDeltaTime() * 0.5f,
				myTestTransform.GetRotation().z);
			Renderer::Submit(myModelsTest[0]->GetVertexArray(), myTestTransform.GetMatrix());


			myWindow->GetDX11().BeginFrame({ 0.8f,0.4f,0.2f,1 });
			Renderer::Begin(myScene->GetMainCamera());
			Renderer::Render();
			Renderer::End();
			myWindow->GetDX11().EndFrame();
		}
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