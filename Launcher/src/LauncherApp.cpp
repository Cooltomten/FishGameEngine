#include "LauncherApp.h"
#include <FGE/Rendering/Renderer.h>

#include <FGE/Asset/ResourceCache.h>
#include <FGE/Asset/Mesh.h>
#include <FGE/Asset/AnimatedMesh.h>
#include <FGE/Asset/Animation.h>
#include <FGE/Asset/Material.h>

#include <CommonUtilities/InputManager.h>
#include <CommonUtilities/UtilityFunctions.hpp>

#include <ImGui/imgui.h>



LauncherApp::LauncherApp(const FGE::WindowProperties& aProperties)
	:FGE::Application(aProperties)
{
	myGame = CreateGame();

	myCamera = std::make_shared<FGE::Camera>();
	myCamera->SetPerspectiveProjection(90, { 1280,720 }, 0.1f, 100000.0f);
	myCameraTransform.SetPosition(0, 0, -100);
	myCamera->RecalculateViewMatrix(myCameraTransform.GetMatrix());

	myCubeMesh = FGE::ResourceCache::GetAsset<FGE::Mesh>("Cube");
	
	
	myChestMesh = FGE::ResourceCache::GetAsset<FGE::Mesh>("Assets/Meshes/SM_Particle_Chest.fbx");

	myChestMaterial = std::make_shared<FGE::Material>();
	myChestMaterial->SetTexture(FGE::MaterialTextureChannel::Albedo, FGE::ResourceCache::GetAsset<FGE::Texture>("Assets/Textures/T_Particle_Chest_C.dds"));
	myChestMesh->SetMaterial(myChestMaterial, 0);

	
	myGremlinMesh = FGE::ResourceCache::GetAsset<FGE::AnimatedMesh>("Assets/Animations/Gremlin/gremlin_sk.fbx");

	myGremlinMaterial = std::make_shared<FGE::Material>();
	myGremlinMaterial->SetTexture(FGE::MaterialTextureChannel::Albedo, FGE::ResourceCache::GetAsset<FGE::Texture>("Assets/Textures/T_gremlin_C.dds"));
	myGremlinMesh->SetMaterial(myGremlinMaterial, 0);

	myGremlinWalkAnim = FGE::ResourceCache::GetAsset<FGE::Animation>("Assets/Animations/Gremlin/gremlin_walk.fbx");
	myGremlinRunAnim = FGE::ResourceCache::GetAsset<FGE::Animation>("Assets/Animations/Gremlin/gremlin_run.fbx");
	

	myCubeTransform.SetPosition(-50,0,0);
	myCubeTransform.SetScale(0.2f, 0.2f, 0.2f);

	myChestTransform.SetPosition({ 0,0,0 });
	myChestTransform.SetScale(0.2f,0.2f,0.2f);
	
	myGremlinTransform.SetPosition({ 50,0,0 });


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
	dispatcher.Dispatch<FGE::AppUpdateEvent>(BIND_EVENT_FN(LauncherApp::OnUpdateEvent));
	dispatcher.Dispatch<FGE::AppRenderEvent>(BIND_EVENT_FN(LauncherApp::OnRenderEvent));
}

bool LauncherApp::OnUpdateEvent(FGE::AppUpdateEvent& aEvent)
{


	myMaterialFadeTimer += aEvent.GetTimeStep();
	myGremlinTimer += aEvent.GetTimeStep() * myAnimationTimeStepMultiplier;
	float duration = CU::Lerp(myGremlinWalkAnim->GetDuration(), myGremlinRunAnim->GetDuration(), myGremlinAlphaBlend);
	if(myGremlinTimer >= duration)
	{
		myGremlinTimer = 0;
	}
	if (myGremlinTimer < 0)
	{
		myGremlinTimer = duration;
	}

	CameraController(aEvent.GetTimeStep());

	//float sinValue = 0.5 * (float)sin(myMaterialFadeTimer * 3) + 0.5;
	//float cosValue = 0.5 * (float)cos(myMaterialFadeTimer * 3) + 0.5;
	//myMaterial->SetAlbedo({ sinValue * cosValue,cosValue,sinValue * sinValue });
	return false;
}

bool LauncherApp::OnRenderEvent(FGE::AppRenderEvent& aEvent)
{

	FGE::Renderer::Begin(myCamera);

	myCubeMesh->Render(myCubeTransform.GetMatrix());
	myChestMesh->Render(myChestTransform.GetMatrix());
	myGremlinMesh->Render(myGremlinTransform.GetMatrix(), myGremlinWalkAnim,myGremlinRunAnim, myGremlinAlphaBlend, myGremlinTimer);

	FGE::Renderer::Render();
	FGE::Renderer::End();

	ImGui::Begin("Test Animations");

	ImGui::Text("Animation Blend Alpha");
	ImGui::DragFloat("##Animation Blend Alpha", &myGremlinAlphaBlend, 0.01f, 0, 1);
	
	ImGui::Text("Animation Time Step Multiplier");
	ImGui::DragFloat("##Animation Time Step Multiplier", &myAnimationTimeStepMultiplier, 0.01f,0);

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

		CU::Vector3f cameraRot = myCameraTransform.GetRotation() + CU::Vector3f(mouseDelta.y, mouseDelta.x, 0) * 0.005f;

		//Clamp camera rotation to prevent camera from flipping over (degrees)
		cameraRot.x = CU::Clamp(cameraRot.x, -89.f, 89.f);


		myCameraTransform.SetRotation(cameraRot);

	}


	myCamera->RecalculateViewMatrix(myCameraTransform.GetMatrix());

}
