#pragma once
#include "Windows/EditorWindow.h"
#include <CommonUtilities/Math/Vector4.hpp>
#include <memory>
#include <ImGui/imgui.h>

class EditorPreferencesWindow : public EditorWindow
{
public:
	EditorPreferencesWindow();
	~EditorPreferencesWindow() override = default;

	virtual void Update(float aTimeStep) override;
	virtual void Render() override;
	virtual void UpdateImGui() override;
private:
	ImVec4 myColor;
};