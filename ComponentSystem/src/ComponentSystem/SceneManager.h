#pragma once
#include <memory>
#include <string>
#include "Scene.h"
namespace Comp
{
	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		static void Initialize();
		static SceneManager& Get();
		static std::shared_ptr<Scene> GetCurrentScene();

		
		void NewScene();
		void LoadScene(const std::string& aScenePath);

	private:
		static SceneManager* myInstance;

		std::shared_ptr<Scene> myCurrentScene;
	};
}