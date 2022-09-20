#pragma once
#include"EditorSettings.h"

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

	LRESULT WindowsMessages(HWND aHwnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam) override;

	void UpdateBackgroundColorBlend();

	bool OnUpdateEvent(FGE::AppUpdateEvent& aEvent);
	bool OnRenderEvent(FGE::AppRenderEvent& aEvent);


	std::vector<std::shared_ptr<EditorWindow>> myWindows;
	std::vector<std::shared_ptr<Viewport>> myViewportWindows;

	FGE::Game* myGame = nullptr;

	std::shared_ptr<FGE::DirectionalLight> myDirectionalLight;
	Comp::Transform myDirectionalLightTransform;

	std::shared_ptr<FGE::EnvironmentLight> myEnvironmentLight;

	std::vector<std::string> myRenderModesStrings;

	std::vector<std::shared_ptr<Comp::Entity>> mySelectedEntities;


	const std::string myEditorSettingsPath = "User/EditorSettings.json";
	EditorSettings myEditorSettings;


	CommonUtilities::Vector4f myFirstBGP;
	CommonUtilities::Vector4f mySecondBGP;

	std::string myFirstBGPPath;
	std::string mySecondBGPPath;

	float myBGPBlendAmount = 0;




};