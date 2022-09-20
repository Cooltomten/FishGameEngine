#pragma once
#include <Engine/Core/Game.h>
#include <Engine/Core/Application.h>
#include <Engine/Event/ApplicationEvents.h>
#include <FGE/Rendering/Camera/Camera.h>

#include <ComponentSystem/Transform.h>

extern FGE::Game* CreateGame();

namespace FGE
{
	class Mesh;
	class AnimatedMesh;
	class Animation;
	class Material;
	class DirectionalLight;
	class EnvironmentLight;
	class ParticleEmitter;
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
	
	std::shared_ptr<FGE::Camera> myCamera;
	Comp::Transform myCameraTransform;


	std::shared_ptr<FGE::DirectionalLight> myDirectionalLight;
	Comp::Transform myDirectionalLightTransform;

	std::shared_ptr<FGE::EnvironmentLight> myEnvironmentLight;

	std::vector<std::string> myRenderModesStrings;
};