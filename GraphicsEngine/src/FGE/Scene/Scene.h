#pragma once
#include <memory>
#include <vector>

namespace FGE
{
	class Camera;
	class Entity;
	class Scene
	{
	public:

		void AddEntity(std::shared_ptr<Entity> aEntity);
		void RemoveEntity(std::shared_ptr<Entity> aEntity);
		
		std::vector<std::shared_ptr<Entity>> GetEntities();

		void SetMainCamera(std::shared_ptr<Camera> aCamera);

		std::shared_ptr<Camera> GetMainCamera();
	private:
		std::shared_ptr<Camera> myMainCamera;
		std::vector<std::shared_ptr<Entity>> myEntities;
	};

}