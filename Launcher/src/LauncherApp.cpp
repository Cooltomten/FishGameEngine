#include "LauncherApp.h"
#include <FGE/Rendering/Renderer.h>
#include <FGE/Rendering/Lights/DirectionalLight.h>
#include <FGE/Rendering/Lights/EnvironmentLight.h>

#include <FGE/Asset/ResourceCache.h>
#include <FGE/Asset/Mesh.h>
#include <FGE/Asset/AnimatedMesh.h>
#include <FGE/Asset/Animation.h>
#include <FGE/Asset/Material.h>

#include <FGE/Particles/ParticleEmitter.h>

#include <CommonUtilities/InputManager.h>
#include <CommonUtilities/UtilityFunctions.hpp>

#include <ComponentSystem/SceneManager.h>
#include <ComponentSystem/Scene.h>


#include <ImGui/imgui.h>



LauncherApp::LauncherApp(const FGE::WindowProperties& aProperties)
	:FGE::Application(aProperties)
{
	//Default,
	//UV1,
	//UV2,
	//UV3,
	//UV4,
	//VertexColors1,
	//VertexColors2,
	//VertexColors3,
	//VertexColors4,
	//VertexNormal,
	//PixelNormal,
	//AlbedoMap,
	//AmbientOcclusion,
	//Roughness,
	//Metalness,
	//Emissiveness,
	//NormalMap,
	//DiffuseLight,
	//AmbientLight,
	// 		DiffuseNoAlbedo,
	//AmbientNoAlbedo
	//COUNT

	myRenderModesStrings.emplace_back("Default");
	myRenderModesStrings.emplace_back("UV 1");
	myRenderModesStrings.emplace_back("UV 2");
	myRenderModesStrings.emplace_back("UV 3");
	myRenderModesStrings.emplace_back("UV 4");
	myRenderModesStrings.emplace_back("Vertex Colors 1");
	myRenderModesStrings.emplace_back("Vertex Colors 2");
	myRenderModesStrings.emplace_back("Vertex Colors 3");
	myRenderModesStrings.emplace_back("Vertex Colors 4");
	myRenderModesStrings.emplace_back("Vertex Normal");
	myRenderModesStrings.emplace_back("Pixel Normal");
	myRenderModesStrings.emplace_back("Albedo Map");
	myRenderModesStrings.emplace_back("Ambient Occlusion");
	myRenderModesStrings.emplace_back("Roughness");
	myRenderModesStrings.emplace_back("Metalness");
	myRenderModesStrings.emplace_back("Emissiveness");
	myRenderModesStrings.emplace_back("Normal Map");
	myRenderModesStrings.emplace_back("Diffuse");
	myRenderModesStrings.emplace_back("Ambient");
	myRenderModesStrings.emplace_back("Diffuse No Albedo");
	myRenderModesStrings.emplace_back("Ambient No Albedo");
	
	myGame = CreateGame();

	myCamera = std::make_shared<FGE::Camera>();
	myCamera->SetPerspectiveProjection(90, { 1280,720 }, 0.1f, 100000.0f);
	myCameraTransform.SetPosition(0, 0, -100);
	myCamera->RecalculateViewMatrix(myCameraTransform.GetMatrix());

	myDirectionalLight = std::make_shared<FGE::DirectionalLight>();
	myDirectionalLight->Init({ 1,1,1 }, 1);

	myEnvironmentLight = std::make_shared<FGE::EnvironmentLight>();
	myEnvironmentLight->SetCubeMapTexture(FGE::ResourceCache::GetAsset<FGE::Texture>("Assets/Textures/CubeMaps/skansen_cubemap.dds"));

	FGE::Renderer::SetDirectionalLight(myDirectionalLight);
	FGE::Renderer::SetEnvironmentLight(myEnvironmentLight);
	
	myDirectionalLightTransform.SetRotation(0, 0, 0);
	myDirectionalLight->SetDirection(myDirectionalLightTransform.GetForward());

	Comp::SceneManager::Initialize();
	Comp::SceneManager::Get().LoadScene("TestSave.scene");
	


	myGame->OnStart();
}

LauncherApp::~LauncherApp()
{
	myGame->OnStop();
	delete myGame;
}

void LauncherApp::OnEventSub(FGE::Event& aEvent)
{
	FGE::EventDispatcher dispatcher(aEvent);
	Comp::SceneManager::GetCurrentScene()->OnEvent(aEvent);
	dispatcher.Dispatch<FGE::AppUpdateEvent>(BIND_EVENT_FN(LauncherApp::OnUpdateEvent));
	dispatcher.Dispatch<FGE::AppRenderEvent>(BIND_EVENT_FN(LauncherApp::OnRenderEvent));
}

bool LauncherApp::OnUpdateEvent(FGE::AppUpdateEvent& aEvent)
{


	CameraController(aEvent.GetTimeStep());

	return false;
}

bool LauncherApp::OnRenderEvent(FGE::AppRenderEvent& aEvent)
{

	FGE::Renderer::Begin(myCamera);

	FGE::Renderer::Render();
	FGE::Renderer::End();

	ImGui::Begin("Stuff n things");

	if (ImGui::BeginCombo("RenderMode", myRenderModesStrings[static_cast<uint32_t>(FGE::Renderer::GetRenderMode())].c_str()))
	{
		for (int i = 0; i < myRenderModesStrings.size(); ++i)
		{
			if (ImGui::Selectable(myRenderModesStrings[i].c_str()))
			{
				FGE::Renderer::SetRenderMode(static_cast<FGE::RenderMode>(i));
			}
		}
		ImGui::EndCombo();
	}


	if (ImGui::Button("BANG"))
	{
		FGE::Window::Get().Resize(1920, 1080);
	}
	//pow 1280x720
	if (ImGui::Button("POW"))
	{
		FGE::Window::Get().Resize(1280, 720);
	}


	ImGui::End();
	
	return false;
}

void LauncherApp::CameraController(float aTimeStep)
{
	auto input = FGE::Application::Get().GetInput();

	CU::Vector2f moveXZInput = { 0,0 };
	float moveYInput = 0;

	if (input->GetKey(Keys::W))
	{
		moveXZInput.y += 1.f;
	}
	if (input->GetKey(Keys::S))
	{
		moveXZInput.y -= 1.f;
	}
	if (input->GetKey(Keys::A))
	{
		moveXZInput.x -= 1.f;
	}
	if (input->GetKey(Keys::D))
	{
		moveXZInput.x += 1.f;
	}

	if (moveXZInput.LengthSqr() > 0)
	{
		moveXZInput.Normalize();
	}

	//go up if space is pressed, go down if ctrl is pressed
	if (input->GetKey(Keys::Space))
	{
		moveYInput += 1.f;
	}
	if (input->GetKey(Keys::Control))
	{
		moveYInput -= 1.f;
	}


	//Roatate camera with mouse delta
	if (input->GetKey(Keys::RightMouse))
	{

		CU::Vector3f cameraPos = myCameraTransform.GetPosition();
		CU::Vector3f forward = myCameraTransform.GetForward();
		CU::Vector3f right = myCameraTransform.GetRight();
		CU::Vector3f up = myCameraTransform.GetUp();

		cameraPos += forward * moveXZInput.y * aTimeStep * 500.f;
		cameraPos += right * moveXZInput.x * aTimeStep * 500.f;
		cameraPos += up * moveYInput * aTimeStep * 500.f;


		myCameraTransform.SetPosition(cameraPos);


		CU::Vector2f mouseDelta = { static_cast<float>(input->GetMousePosDelta().x), static_cast<float>(input->GetMousePosDelta().y) };

		CU::Vector3f cameraRot = myCameraTransform.GetRotation() + CU::Vector3f(mouseDelta.y, mouseDelta.x, 0) * 0.05f;

		//Clamp camera rotation to prevent camera from flipping over (degrees)
		cameraRot.x = CU::Clamp(cameraRot.x, -89.f, 89.f);


		myCameraTransform.SetRotation(cameraRot);

	}


	myCamera->RecalculateViewMatrix(myCameraTransform.GetMatrix());

}
