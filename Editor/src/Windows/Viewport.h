#pragma once
#include "Windows/EditorWindow.h"
#include <memory>
namespace FGE
{
	class RenderTexture;
	class Camera;
}
class SceneCamera;
class Viewport : public EditorWindow
{
public:
	Viewport();
	~Viewport() override  = default;
	// Inherited via EditorWindow
	void Update(float aTimeStep) override;
	void Render() override;

	inline std::shared_ptr<SceneCamera> GetSceneCamera() { return mySceneCamera; }
	inline std::shared_ptr<FGE::RenderTexture> GetRenderTexture() { return myRenderTexture; }
private:
	std::shared_ptr<FGE::RenderTexture> myRenderTexture;
	std::shared_ptr<SceneCamera> mySceneCamera;

	unsigned int myWindowWidth;
	unsigned int myWindowHeight;
	
};