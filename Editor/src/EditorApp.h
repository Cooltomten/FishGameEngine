#pragma once
#include <Engine/Core/Game.h>
#include <Engine/Core/Application.h>
#include <Engine/Event/ApplicationEvents.h>
#include <FGE/Rendering/Camera/Camera.h>
#include <FGE/Rendering/RenderTexture.h>

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

namespace Comp
{
	class Entity;
}

class EditorWindow;
class Viewport;
class SceneCamera;
class EditorApp : public FGE::Application
{
public:
	EditorApp(const FGE::WindowProperties& aProperties);
	~EditorApp();

	void OnEventSub(FGE::Event& aEvent) override;

private:
	bool OnUpdateEvent(FGE::AppUpdateEvent& aEvent);
	bool OnRenderEvent(FGE::AppRenderEvent& aEvent);
	

	std::vector<std::shared_ptr<EditorWindow>> myWindows;
	std::vector<std::shared_ptr<Viewport>> myViewportWindows;

		FGE::Game* myGame = nullptr;


	float myMaterialFadeTimer = 0;

	std::shared_ptr<FGE::Mesh> myCubeMesh;
	Comp::Transform myCubeTransform;
	//std::shared_ptr<FGE::Mesh> myChestMesh;
	//Comp::Transform myChestTransform;
	//std::shared_ptr<FGE::Material> myChestMaterial;

	std::shared_ptr<FGE::AnimatedMesh> myGremlinMesh;
	Comp::Transform myGremlinTransform;
	float myGremlinTimer = 0;
	float myGremlinAlphaBlend = 0;
	float myAnimationTimeStepMultiplier = 1;
	std::shared_ptr<FGE::Animation> myGremlinWalkAnim;
	std::shared_ptr<FGE::Animation> myGremlinRunAnim;
	std::shared_ptr<FGE::Material> myGremlinMaterial;

	std::shared_ptr<FGE::DirectionalLight> myDirectionalLight;
	Comp::Transform myDirectionalLightTransform;

	std::shared_ptr<FGE::EnvironmentLight> myEnvironmentLight;

	std::vector<std::string> myRenderModesStrings;

	std::shared_ptr<FGE::RenderTexture> myRenderTexture;
	std::shared_ptr<SceneCamera> myCamera;

	std::shared_ptr<FGE::ParticleEmitter> myParticles;

	std::vector<std::shared_ptr<Comp::Entity>> mySelectedEntities;
	

	
	

};