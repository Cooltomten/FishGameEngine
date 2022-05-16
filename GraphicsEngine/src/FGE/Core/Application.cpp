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
		camera->SetPerspectiveProjection(90, { 1280,720 }, 0.1f, 100000.0f);
		camera->GetTransform().SetPosition({ 0,0,-200 });
		myScene->SetMainCamera(camera);

		myModelsTest.push_back(ResourceCache::GetAsset("Assets/Meshes/SM_Particle_Chest.fbx"));

		myTestTransform.SetPosition({ 0,0,0 });
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

			CameraController();

			std::cout << myInputManager->GetMousePosDelta().x << " " << myInputManager->GetMousePosDelta().y << std::endl;


			myModelsTest[0]->Render(myTestTransform.GetMatrix());


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

	void Application::CameraController()
	{
		CU::Vector2f moveInput;

		if (myInputManager->GetKey(Keys::W))
		{
			moveInput.y += 1.f;
		}
		if (myInputManager->GetKey(Keys::S))
		{
			moveInput.y -= 1.f;
		}
		if (myInputManager->GetKey(Keys::A))
		{
			moveInput.x -= 1.f;
		}
		if (myInputManager->GetKey(Keys::D))
		{
			moveInput.x += 1.f;
		}

		if (moveInput.LengthSqr() > 0)
		{
			moveInput.Normalize();
		}


		//Roatate camera with mouse delta
		if (myInputManager->GetKey(Keys::RightMouse))
		{
			FGE::Transform& cameraTransform = myScene->GetMainCamera()->GetTransform();

			CU::Vector3f cameraPos = cameraTransform.GetPosition();
			CU::Vector3f forward = cameraTransform.GetForward();
			CU::Vector3f right = cameraTransform.GetRight();

			cameraPos += forward * moveInput.y * myTimer->GetDeltaTime() * 500.f;
			cameraPos += right * moveInput.x * myTimer->GetDeltaTime() * 500.f;

			cameraTransform.SetPosition(cameraPos);


			CU::Vector2f mouseDelta = { static_cast<float>(myInputManager->GetMousePosDelta().x), static_cast<float>(myInputManager->GetMousePosDelta().y) };
			
			cameraTransform.SetRotation(cameraTransform.GetRotation() + CU::Vector3f(mouseDelta.y, mouseDelta.x, 0) * 0.005f);
		}


	}

}