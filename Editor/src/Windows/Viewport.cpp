#include "Viewport.h"
#include "Engine/Core/Application.h"

#include "SceneCamera.h"

#include <FGE/Rendering/RenderTexture.h>
#include <ImGui/imgui.h>
#include <string>
#include <iostream>

#include <d3d11.h>

Viewport::Viewport()
	: EditorWindow("Viewport")
{
	mySceneCamera = std::make_shared<SceneCamera>();

	myRenderTexture = std::make_shared<FGE::RenderTexture>();
	auto& dx11 = FGE::Application::Get().GetWindow()->GetDX11();
	if (!myRenderTexture->Initialize(dx11.GetDevice(), 1280, 720))
	{
		myRenderTexture = nullptr;
	}
}

void Viewport::Update(float aTimeStep)
{
	mySceneCamera->Update(aTimeStep);
}

void Viewport::Render()
{

}

void Viewport::UpdateImGui()
{
	if (ImGui::Begin((std::string("Viewport##") + std::to_string(myId)).c_str(), &myOpenFlag))
	{
		if (ImGui::GetContentRegionAvail().x != myWindowWidth || ImGui::GetContentRegionAvail().y != myWindowHeight)
		{
			myWindowWidth = ImGui::GetContentRegionAvail().x;
			myWindowHeight = ImGui::GetContentRegionAvail().y;
			if(myWindowHeight <= 0)
			{
				myWindowHeight = 1;
			}
			if (myWindowWidth <= 0)
			{
				myWindowWidth = 1;
			}


			mySceneCamera->Resize(myWindowWidth, myWindowHeight);
			myRenderTexture->Initialize(FGE::Application::Get().GetWindow()->GetDX11().GetDevice(), myWindowWidth, myWindowHeight);
		}

		auto& dx11 = FGE::Application::Get().GetWindow()->GetDX11();
		auto drawList = ImGui::GetWindowDrawList();

		ImGui::Image((void*)myRenderTexture->GetShaderResourceView().Get(), { (float)myWindowWidth, (float)myWindowHeight });
	}
	ImGui::End();
}
