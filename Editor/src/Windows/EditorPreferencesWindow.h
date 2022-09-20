#pragma once
#include "Windows/EditorWindow.h"
#include <CommonUtilities/Math/Vector4.hpp>
#include <memory>
#include <ImGui/imgui.h>
#include <EditorSettings.h>

class EditorPreferencesWindow : public EditorWindow
{
public:
	EditorPreferencesWindow() = delete;
	EditorPreferencesWindow(EditorSettings& aEditorSettings);
	~EditorPreferencesWindow() override = default;

	virtual void Update(float aTimeStep) override;
	virtual void Render() override;
	virtual void UpdateImGui() override;

	void OnEvent(FGE::Event& aEvent) override;
private:
	ImVec4 myColor;
	EditorSettings& myEditorSettings;
};