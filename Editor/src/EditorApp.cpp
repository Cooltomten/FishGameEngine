#include "EditorApp.h"

#include "Windows/EditorWindow.h"
#include "Windows/Viewport.h"
#include "Windows/HierarchyWindow.h"
#include "Windows/InspectorWindow.h"
#include "Windows/EditorPreferencesWindow.h"

#include <FGE/Rendering/Renderer.h>
#include <FGE/Rendering/Lights/DirectionalLight.h>
#include <FGE/Rendering/Lights/EnvironmentLight.h>

#include <FGE/Asset/ResourceCache.h>
#include <FGE/Asset/Mesh.h>
#include <FGE/Asset/AnimatedMesh.h>
#include <FGE/Asset/Animation.h>
#include <FGE/Asset/Material.h>

#include <Engine/Components/MeshRenderer.h>

#include <CommonUtilities/InputManager.h>
#include <CommonUtilities/UtilityFunctions.hpp>

#include <ComponentSystem/SceneManager.h>
#include <ComponentSystem/Scene.h>
#include <ComponentSystem/Entity.h>

#include <SceneCamera.h>

#include <ImGui/imgui.h>

#include <nlohmann/json.hpp>


EditorApp::EditorApp(const FGE::WindowProperties& aProperties)
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
	myRenderModesStrings.emplace_back("Normal Map");
	myRenderModesStrings.emplace_back("Diffuse");
	myRenderModesStrings.emplace_back("Ambient");
	myRenderModesStrings.emplace_back("Diffuse No Albedo");
	myRenderModesStrings.emplace_back("Ambient No Albedo");


	myGame = CreateGame();

	myDirectionalLight = std::make_shared<FGE::DirectionalLight>();
	myDirectionalLight->Init({ 1,1,1 }, 1);

	myEnvironmentLight = std::make_shared<FGE::EnvironmentLight>();
	myEnvironmentLight->SetCubeMapTexture(FGE::ResourceCache::GetAsset<FGE::Texture>("Assets/Textures/CubeMaps/skansen_cubemap.dds"));

	FGE::Renderer::SetDirectionalLight(myDirectionalLight);
	FGE::Renderer::SetEnvironmentLight(myEnvironmentLight);

	//Add entities
	Comp::SceneManager::Initialize();
	Comp::SceneManager::Get().NewScene();
	auto scene = Comp::SceneManager::GetCurrentScene();

	std::shared_ptr<Comp::Entity> entity = std::make_shared<Comp::Entity>("One", 1);
	std::shared_ptr<Engine::MeshRenderer> meshRenderer = std::make_shared<Engine::MeshRenderer>();
	entity->AddComponent(meshRenderer);
	Comp::SceneManager::GetCurrentScene()->AddEntity(entity);


	//another entity 
	entity = std::make_shared<Comp::Entity>("Two", 2);
	meshRenderer = std::make_shared<Engine::MeshRenderer>();
	entity->AddComponent(meshRenderer);
	Comp::SceneManager::GetCurrentScene()->AddEntity(entity);

	myDirectionalLightTransform.SetRotation(0, 0, 0);
	myDirectionalLight->SetDirection(myDirectionalLightTransform.GetForward());
	myGame->OnStart();


	if (std::filesystem::exists(myEditorSettingsPath))
	{
		std::ifstream file = std::ifstream(myEditorSettingsPath);
		nlohmann::json json;
		file >> json;
		file.close();

		Comp::SceneManager::Get().LoadScene(json["LastLoadedScene"]);
	}


	//Create a viewport by default
	myViewportWindows.push_back(std::make_shared<Viewport>());
	myWindows.push_back(myViewportWindows.back());
	myWindows.back()->SetOpen(true);

	//Create a HierarchyWindow by default
	myWindows.push_back(std::make_shared<HierarchyWindow>(mySelectedEntities));
	myWindows.back()->SetOpen(true);

	//create InspectorWindow
	myWindows.push_back(std::make_shared<InspectorWindow>(mySelectedEntities));
	myWindows.back()->SetOpen(true);


}

EditorApp::~EditorApp()
{
	myGame->OnStop();
	delete myGame;
}

void EditorApp::OnEventSub(FGE::Event& aEvent)
{
	FGE::EventDispatcher dispatcher(aEvent);
	Comp::SceneManager::GetCurrentScene()->OnEvent(aEvent);
	dispatcher.Dispatch<FGE::AppUpdateEvent>(BIND_EVENT_FN(EditorApp::OnUpdateEvent));
	dispatcher.Dispatch<FGE::AppRenderEvent>(BIND_EVENT_FN(EditorApp::OnRenderEvent));
}

LRESULT EditorApp::WindowsMessages(HWND aHwnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam)
{
	switch (aMessage)
	{
	case WM_CLOSE:
	{
		nlohmann::json json;
		json["LastLoadedScene"] = Comp::SceneManager::Get().GetCurrentScenePath().string();

		if (!std::filesystem::exists("User"))
		{
			std::filesystem::create_directory("User");
		}
		std::ofstream file;
		file.open(myEditorSettingsPath);
		file << json;
		file.close();
	}
	break;
	}
	return 0;
}

bool EditorApp::OnUpdateEvent(FGE::AppUpdateEvent& aEvent)
{
	ImGui::DockSpaceOverViewport();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
				//TODO: Popup to check if user wants to save current scene
				Comp::SceneManager::Get().NewScene();
			}
			if (ImGui::MenuItem("Load"))
			{
				//TODO: Popup to check if user wants to save current scene
				//TODO: get path from user
				Comp::SceneManager::Get().LoadScene("TestSave.scene");
			}
			if (ImGui::MenuItem("Save", "Ctrl + S"))
			{
				Comp::SceneManager::Get().SaveCurrentScene("TestSave.scene");
			}
			if (ImGui::MenuItem("Save As", "Ctrl + Shift + S"))
			{
				//TODO: get path from user
				//Comp::SceneManager::Get().SaveCurrentSceneAs();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Editor Preferences"))
			{
				bool shouldCreate = true;
				for (auto& window : myWindows)
				{
					if (!window->IsOpen())
					{
						window->SetOpen(true);
						shouldCreate = false;
						break;
					}

				}

				if (shouldCreate)
				{
					myWindows.push_back(std::make_shared<EditorPreferencesWindow>());
					myWindows.back()->SetOpen(true);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			//TODO: Refactor this
			if (ImGui::MenuItem("Viewport"))
			{
				bool shouldCreate = true;
				for (auto& window : myWindows)
				{
					if (!window->IsOpen())
					{
						window->SetOpen(true);
						shouldCreate = false;
						break;
					}

				}

				if (shouldCreate)
				{
					myViewportWindows.push_back(std::make_shared<Viewport>());
					myWindows.push_back(myViewportWindows.back());
					myWindows.back()->SetOpen(true);
				}
			}

			if (ImGui::MenuItem("Hierarchy"))
			{
				bool shouldCreate = true;
				for (auto& window : myWindows)
				{
					if (!window->IsOpen())
					{
						window->SetOpen(true);
						shouldCreate = false;
						break;
					}

				}

				if (shouldCreate)
				{
					myWindows.push_back(std::make_shared<HierarchyWindow>(mySelectedEntities));
					myWindows.back()->SetOpen(true);
				}
			}

			//Inspector
			if (ImGui::MenuItem("Inspector"))
			{
				bool shouldCreate = true;
				for (auto& window : myWindows)
				{
					if (!window->IsOpen())
					{
						window->SetOpen(true);
						shouldCreate = false;
						break;
					}

				}

				if (shouldCreate)
				{
					myWindows.push_back(std::make_shared<InspectorWindow>(mySelectedEntities));
					myWindows.back()->SetOpen(true);
				}
			}



			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		
	}

	for (int i = 0; i < myWindows.size(); i++)
	{
		if (myWindows[i]->IsOpen())
		{
			myWindows[i]->Update(aEvent.GetTimeStep());
		}
	}
	return false;
}

bool EditorApp::OnRenderEvent(FGE::AppRenderEvent& aEvent)
{
	auto& dx11 = Application::Get().GetWindow()->GetDX11();

	for (int i = 0; i < myWindows.size(); i++)
	{
		if (myWindows[i]->IsOpen())
		{
			myWindows[i]->UpdateImGui();
		}
	}

	for (int i = 0; i < myViewportWindows.size(); i++)
	{

			auto clearColor = FGE::Window::Get().GetDX11().GetClearColor();
		if (myViewportWindows[i]->IsOpen())
		{

			FGE::Renderer::Begin(myViewportWindows[i]->GetSceneCamera()->GetCamera());

			//Set render target
			FGE::Renderer::SetRenderTarget(myViewportWindows[i]->GetRenderTexture()->GetRenderTargetData());
			myViewportWindows[i]->GetRenderTexture()->ClearRenderTarget(dx11.GetDeviceContext(), clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
			FGE::Renderer::Render();
		}
	}
			FGE::Renderer::End();

	dx11.SetRenderTarget();

	for (int i = 0; i < myWindows.size(); i++)
	{
		if (myWindows[i]->IsOpen())
		{
			myWindows[i]->Render();
		}
	}






	return false;
}

