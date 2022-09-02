#pragma once
#include <memory>
#include <string>
#include "Scene.h"
#include <filesystem>
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
		void SaveCurrentScene(const std::filesystem::path& aPath);
	private:
		static SceneManager* myInstance;

		std::shared_ptr<Scene> myCurrentScene;
	};
}