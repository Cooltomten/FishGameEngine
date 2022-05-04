#include "GraphicsEngine.pch.h"
#include "Application.h"
#include "FGE/Rendering/Camera/Camera.h"
#include "FGE/Asset/ResourceCache.h"

namespace FGE
{
	Application* Application::myInstance = nullptr;

	Application::Application(const WindowProperties& aProperties)
	{
		myInstance = this;
		myWindow = Window::Create(aProperties);
		try
		{
			myForwardRenderer.Initialize();
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
		myModelsTest[0]->GetTransform().SetRotation({0,0.8,0});
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

			myWindow->GetDX11().BeginFrame({ 0.8f,0.4f,0.2f,1 });

			myModelsTest[0]->GetTransform().SetRotation({ myModelsTest[0]->GetTransform().GetRotation().x + 0.01f,
				myModelsTest[0]->GetTransform().GetRotation().y + 0.01f,
				myModelsTest[0]->GetTransform().GetRotation().z + 0.01f });
			
			myForwardRenderer.Render(myScene->GetMainCamera(), myModelsTest);

			myWindow->GetDX11().EndFrame();
		}
	}

}