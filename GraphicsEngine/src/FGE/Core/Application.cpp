#include "GraphicsEngine.pch.h"
#include "Application.h"

#include "FGE/Asset/ResourceCache.h"
#include "FGE/Asset/Material.h"
#include "FGE/Asset/AnimatedMesh.h"

#include "FGE/Rendering/Camera/Camera.h"
#include "FGE/Rendering/Renderer.h"

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


		myScene = new Scene();
		std::shared_ptr<Camera> camera = std::make_shared<Camera>();
		camera->SetPerspectiveProjection(90, { 1280,720 }, 0.1f, 100000.0f);
		camera->GetTransform().SetPosition({ 0,0,-200 });
		myScene->SetMainCamera(camera);

		myChestMesh = ResourceCache::GetAsset<Mesh>("Assets/Meshes/SM_Particle_Chest.fbx");
		std::shared_ptr<FGE::Material> material = std::make_shared<FGE::Material>();
		material->SetAlbedo({ 0,0,0.5 });
		myChestMesh->SetMaterial(material, 0);

		myGremlinMesh = ResourceCache::GetAsset<AnimatedMesh>("Assets/Animations/Gremlin/gremlin_sk.fbx");
		myGremlinMesh->SetMaterial(material, 0);

		myChestTransform.SetPosition({ 0,0,0 });
		myGremlinTransform.SetPosition({ 200,0,0 });
	}

	Application::~Application()
	{
		delete myScene;
	}

	void Application::Run()
	{
		myRunning = true;
		std::shared_ptr<FGE::Material> material = myChestMesh->GetMaterial(0);
		while (myRunning)
		{
			Window::ProcessMessages();

			myTimer->Update();
			myInputManager->UpdateInput();

			CameraController();

			std::cout << myInputManager->GetMousePosDelta().x << " " << myInputManager->GetMousePosDelta().y << std::endl;


			//Fade material albedo to random color with myTimer.GetTotalTime()
			float sinValue = 0.5 * (float)sin(myTimer->GetTotalTime() * 3) + 0.5;
			float cosValue = 0.5 * (float)cos(myTimer->GetTotalTime() * 3) + 0.5;
			material->SetAlbedo({ sinValue * cosValue,cosValue,sinValue * sinValue });

			myChestMesh->Render(myChestTransform.GetMatrix());
			myGremlinMesh->Render(myGremlinTransform.GetMatrix());


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
		CU::Vector2f moveXZInput = { 0,0 };
		float moveYInput = 0;

		if (myInputManager->GetKey(Keys::W))
		{
			moveXZInput.y += 1.f;
		}
		if (myInputManager->GetKey(Keys::S))
		{
			moveXZInput.y -= 1.f;
		}
		if (myInputManager->GetKey(Keys::A))
		{
			moveXZInput.x -= 1.f;
		}
		if (myInputManager->GetKey(Keys::D))
		{
			moveXZInput.x += 1.f;
		}

		if (moveXZInput.LengthSqr() > 0)
		{
			moveXZInput.Normalize();
		}

		//go up if space is pressed, go down if ctrl is pressed
		if (myInputManager->GetKey(Keys::Space))
		{
			moveYInput += 1.f;
		}
		if (myInputManager->GetKey(Keys::Control))
		{
			moveYInput -= 1.f;
		}


		//Roatate camera with mouse delta
		if (myInputManager->GetKey(Keys::RightMouse))
		{
			FGE::Transform& cameraTransform = myScene->GetMainCamera()->GetTransform();

			CU::Vector3f cameraPos = cameraTransform.GetPosition();
			CU::Vector3f forward = cameraTransform.GetForward();
			CU::Vector3f right = cameraTransform.GetRight();
			CU::Vector3f up = cameraTransform.GetUp();

			cameraPos += forward * moveXZInput.y * myTimer->GetDeltaTime() * 500.f;
			cameraPos += right * moveXZInput.x * myTimer->GetDeltaTime() * 500.f;
			cameraPos += up * moveYInput * myTimer->GetDeltaTime() * 500.f;


			cameraTransform.SetPosition(cameraPos);


			CU::Vector2f mouseDelta = { static_cast<float>(myInputManager->GetMousePosDelta().x), static_cast<float>(myInputManager->GetMousePosDelta().y) };

			CU::Vector3f cameraRot = cameraTransform.GetRotation() + CU::Vector3f(mouseDelta.y, mouseDelta.x, 0) * 0.005f;

			//Clamp camera rotation to prevent camera from flipping over (degrees)
			cameraRot.x = CU::Clamp(cameraRot.x, -89.f, 89.f);


			cameraTransform.SetRotation(cameraRot);

		}


	}

}