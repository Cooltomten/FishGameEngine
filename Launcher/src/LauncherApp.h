#pragma once
#include <FGE/Core/Game.h>
#include <FGE/Core/Application.h>
#include <FGE/Event/ApplicationEvents.h>
#include <FGE/Rendering/Camera/Camera.h>

#include <ComponentSystem/Transform.h>

extern FGE::Game* CreateGame();

namespace FGE
{
	class Mesh;
	class AnimatedMesh;
	class Animation;
	class Material;
}

class LauncherApp : public FGE::Application
{
public:
	LauncherApp(const FGE::WindowProperties& aProperties);
	~LauncherApp();
	
	void OnEventSub(FGE::Event& aEvent) override;

private:
	bool OnUpdateEvent(FGE::AppUpdateEvent& aEvent);
	bool OnRenderEvent(FGE::AppRenderEvent& aEvent);

	void CameraController(float aTimeStep);

	

	FGE::Game* myGame = nullptr;


	float myMaterialFadeTimer = 0;
	
	std::shared_ptr<FGE::Camera> myCamera;
	Comp::Transform myCameraTransform;

	std::shared_ptr<FGE::Mesh> myCubeMesh;
	Comp::Transform myCubeTransform;

	std::shared_ptr<FGE::Mesh> myChestMesh;
	Comp::Transform myChestTransform;
	std::shared_ptr<FGE::Material> myChestMaterial;

	std::shared_ptr<FGE::AnimatedMesh> myGremlinMesh;
	Comp::Transform myGremlinTransform;
	float myGremlinTimer = 0;
	float myGremlinAlphaBlend = 0;
	float myAnimationTimeStepMultiplier = 1;
	std::shared_ptr<FGE::Animation> myGremlinWalkAnim;
	std::shared_ptr<FGE::Animation> myGremlinRunAnim;
	std::shared_ptr<FGE::Material> myGremlinMaterial;

	

};