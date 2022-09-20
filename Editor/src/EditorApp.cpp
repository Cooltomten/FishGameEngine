#include "EditorApp.h"

#include "Windows/EditorWindow.h"
#include "Windows/Viewport.h"
#include "Windows/HierarchyWindow.h"
#include "Windows/InspectorWindow.h"
#include "Windows/EditorPreferencesWindow.h"

#include "ImGuiUtil.h"

#include "Engine/Event/ApplicationEvents.h"

#include <FGE/Rendering/Renderer.h>
#include <FGE/Rendering/Lights/DirectionalLight.h>
#include <FGE/Rendering/Lights/EnvironmentLight.h>

#include <FGE/Asset/ResourceCache.h>
#include <FGE/Asset/Mesh.h>
#include <FGE/Asset/AnimatedMesh.h>
#include <FGE/Asset/Animation.h>
#include <FGE/Asset/Material.h>

#include "FGE/Core/Window.h"
#include "FGE/Core/DX11.h"

//#include <Engine/Components/MeshRenderer.h>

#include <CommonUtilities/InputManager.h>
#include <CommonUtilities/UtilityFunctions.hpp>

#include <ComponentSystem/SceneManager.h>
#include <ComponentSystem/Scene.h>
#include <ComponentSystem/Entity.h>

#include <SceneCamera.h>

#include <ImGui/imgui.h>

#include <nlohmann/json.hpp>

#include <iostream>

#include <shellapi.h> // for drag drop
#include <WinBase.h> // for copy file


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

	myDirectionalLightTransform.SetRotation(0, 0, 0);
	myDirectionalLight->SetDirection(myDirectionalLightTransform.GetForward());
	myGame->OnStart();


	Comp::SceneManager::Initialize();
	Comp::SceneManager::Get().NewScene();

	myEditorSettings.Load();
	if (myEditorSettings.myLastLoadedScene != "")
	{
		Comp::SceneManager::Get().LoadScene(myEditorSettings.myLastLoadedScene);
	}
	if (myEditorSettings.myLastLoadedBackground != "")
	{
		auto color = myEditorSettings.LoadBackgroundFile(myEditorSettings.myLastLoadedBackground);
		FGE::Window::Get().GetDX11().SetClearColor({ color.x, color.y, color.z, 1 });
	}
	if (myEditorSettings.myLastLoadedFirstBGP != "")
	{
		myFirstBGPPath = myEditorSettings.myLastLoadedFirstBGP;
		myFirstBGP = myEditorSettings.LoadBackgroundFile(myFirstBGPPath);
	}
	if (myEditorSettings.myLastLoadedSecondBGP != "")
	{
		mySecondBGPPath = myEditorSettings.myLastLoadedSecondBGP;
		mySecondBGP = myEditorSettings.LoadBackgroundFile(mySecondBGPPath);
	}
	if (myEditorSettings.myLastBlendValue != -1)
	{
		myBGPBlendAmount = myEditorSettings.myLastBlendValue;
	}

	if (myEditorSettings.myLastLoadedSecondBGP != "" && myEditorSettings.myLastLoadedFirstBGP != "")
	{
	}
	if (myEditorSettings.myBlendActive)
	{
		UpdateBackgroundColorBlend();
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

	for (int i = 0; i < myWindows.size(); ++i)
	{
		myWindows[i]->OnEvent(aEvent);
	}
}

LRESULT EditorApp::WindowsMessages(HWND aHwnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam)
{
	
	switch (aMessage)
	{
	case WM_CLOSE:

		myEditorSettings.myLastLoadedScene = Comp::SceneManager::Get().GetCurrentScenePath().string();
		myEditorSettings.Save();
		break;

	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)aWParam;
		UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		for (UINT i = 0; i < nFiles; ++i)
		{
			char szFileName[MAX_PATH];
			DragQueryFileA(hDrop, i, szFileName, MAX_PATH);
			std::string path = szFileName;
			if (path.find(".dds") != std::string::npos)
			{
				std::string newPath = "Assets/Textures/" + path.substr(path.find_last_of("\\") + 1);
				bool failed = CopyFileA(path.c_str(),
					(newPath).c_str(),
					true);
				if (failed)
				{
					std::cout << "Failed to copy file from: " << path << " to: " << "Assets/Textures/" + path.substr(path.find_last_of("\\") + 1) << std::endl;
					if (std::filesystem::exists(newPath))
					{
						std::cout << "File already exists, Delete the current one before dropping a new one" << std::endl;
					}
				}
			}
		}
		DragFinish(hDrop);

		break;
	}
	}
	return 0;
}

void EditorApp::UpdateBackgroundColorBlend()
{
	auto lerped = CommonUtilities::Lerp(myFirstBGP, mySecondBGP, myBGPBlendAmount);
	FGE::Window::Get().GetDX11().SetClearColor({ lerped.x, lerped.y, lerped.z, lerped.z });
}

bool EditorApp::OnUpdateEvent(FGE::AppUpdateEvent& aEvent)
{
	if (FGE::Window::Get().GetDX11().ClearColorChanged())
	{
		FGE::ClearColorChangedEvent ev(FGE::Window::Get().GetDX11().GetClearColor());
		OnEvent(ev);
	}
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
			if (ImGui::MenuItem("Save"/*, "Ctrl + S"*/))
			{
				Comp::SceneManager::Get().SaveCurrentScene("TestSave.scene");
			}
			//if (ImGui::MenuItem("Save As", "Ctrl + Shift + S"))
			//{
			//	//TODO: get path from user
			//	//Comp::SceneManager::Get().SaveCurrentSceneAs();
			//}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Editor Preferences"))
			{
				bool shouldCreate = true;
				for (auto& window : myWindows)
				{
					if (!window->IsOpen() && window->GetName() == "Editor Preferences")
					{
						window->SetOpen(true);
						shouldCreate = false;
						break;
					}

				}

				if (shouldCreate)
				{
					myWindows.push_back(std::make_shared<EditorPreferencesWindow>(myEditorSettings));
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
					if (!window->IsOpen() && window->GetName() == "Viewport")
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
					if (!window->IsOpen() && window->GetName() == "Hierarchy")
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
					if (!window->IsOpen() && window->GetName() == "Inspector")
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

	if (ImGui::Begin("Blend n stuff"))
	{
		int width = 100;
		int height = 20;
		ImGui::Text("Blend n stuff");
		if (ImGui::Button("Load first background"))
		{
			auto firstPath = ImGuiUtil::GetOpenFilePath("Background Presets (*.bgp) \0*.bgp\0");
			if (firstPath != "")
			{
				myFirstBGPPath = firstPath.string();
				myFirstBGP = myEditorSettings.LoadBackgroundFile(firstPath);
				if (myEditorSettings.myBlendActive)
				{
					UpdateBackgroundColorBlend();
				}
			}

		}
		//Draw first BGP
		ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + width, ImGui::GetCursorScreenPos().y + height),
			ImGui::ColorConvertFloat4ToU32(ImVec4(myFirstBGP.x, myFirstBGP.y, myFirstBGP.z, myFirstBGP.w)));
		ImGui::SetCursorPosY(ImGui::GetCursorPos().y + height + 10);

		if (ImGui::Button("Load second background"))
		{
			auto secondPath = ImGuiUtil::GetOpenFilePath("Background Presets (*.bgp) \0*.bgp\0");
			if (secondPath != "")
			{
				mySecondBGPPath = secondPath.string();
				mySecondBGP = myEditorSettings.LoadBackgroundFile(secondPath);
				if (myEditorSettings.myBlendActive)
				{
					UpdateBackgroundColorBlend();
				}
			}
		}
		//Draw second BGP
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y),
			ImVec2(ImGui::GetCursorScreenPos().x + width, ImGui::GetCursorScreenPos().y + height),
			ImGui::ColorConvertFloat4ToU32(ImVec4(mySecondBGP.x, mySecondBGP.y, mySecondBGP.z, mySecondBGP.w)));
		ImGui::SetCursorPosY(ImGui::GetCursorPos().y + height + 10);

		if (ImGui::DragFloat("BlendPercent##BlendPercentBGP", &myBGPBlendAmount, 0.01f, 0, 1))
		{
			if (myEditorSettings.myBlendActive)
			{
				UpdateBackgroundColorBlend();
			}
		}


		ImGui::Text("Result:");
		//DrawBlend
		auto lerped = CommonUtilities::Lerp(myFirstBGP, mySecondBGP, myBGPBlendAmount);
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y),
			ImVec2(ImGui::GetCursorScreenPos().x + width, ImGui::GetCursorScreenPos().y + height),
			ImGui::ColorConvertFloat4ToU32(ImVec4(lerped.x, lerped.y, lerped.z, lerped.w)));
		ImGui::SetCursorPosY(ImGui::GetCursorPos().y + height + 10);

		if (ImGui::Button("SaveSettings##SaveBlendingSettings"))
		{
			myEditorSettings.myLastLoadedFirstBGP = myFirstBGPPath;
			myEditorSettings.myLastLoadedSecondBGP = mySecondBGPPath;
			myEditorSettings.myLastBlendValue = myBGPBlendAmount;
		}

		if (ImGui::Button("Set As Active Background"))
		{
			myEditorSettings.myBlendActive = true;
			UpdateBackgroundColorBlend();
		}


	}
	ImGui::End();

	for (int i = 0; i < myViewportWindows.size(); i++)
	{

		auto clearColor = FGE::Window::Get().GetDX11().GetClearColor();
		if (myViewportWindows[i]->IsOpen())
		{

			FGE::Renderer::Begin(myViewportWindows[i]->GetSceneCamera()->GetCamera());

			//Set render target
			FGE::Renderer::SetRenderTarget(myViewportWindows[i]->GetRenderTexture()->GetRenderTargetData());
			myViewportWindows[i]->GetRenderTexture()->ClearRenderTarget(dx11.GetDeviceContext(), clearColor[0], clearColor[1], clearColor[2], 1);
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

