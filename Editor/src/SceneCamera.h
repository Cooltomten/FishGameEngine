#pragma once
#include <memory>
#include <CommonUtilities/InputManager.h>
#include <ComponentSystem/Transform.h>
#include <CommonUtilities/Math/Vector.hpp>

namespace CU = CommonUtilities;

namespace FGE
{
	class Camera;
}

class SceneCamera
{
	public:
	SceneCamera();
	~SceneCamera();
	
	void Update(float aTimeStep);
	inline std::shared_ptr<FGE::Camera> GetCamera() { return myCamera; }

	void Resize(unsigned int aWidth, unsigned int aHeight);
	inline CU::Vector2f GetSize() { return mySize; }
	

	private:
		std::shared_ptr<FGE::Camera> myCamera;
		std::shared_ptr<CommonUtilities::InputManager> myInput;
		Comp::Transform myTransform;
		CU::Vector2f mySize;
		
};