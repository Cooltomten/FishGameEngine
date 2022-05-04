#include <FGE/EntryPoint.h>
#include <FGE/Core/Game.h>

extern FGE::Game* CreateGame();

class LauncherApp : public FGE::Application
{
	public:
		LauncherApp(const FGE::WindowProperties& aProperties)
			:FGE::Application(aProperties)
		{
			myGame = CreateGame();
			myGame->OnStart();
		}
		~LauncherApp()
		{
			myGame->OnStop();
			delete myGame;
		}
private:
	FGE::Game* myGame = nullptr;
};

FGE::Application* FGE::CreateApplication()
{
	FGE::WindowProperties windowProps;
	windowProps.X = 100;
	windowProps.Y = 100;
	windowProps.Width = 1280;
	windowProps.Height = 720;
	windowProps.IsVSync= false;
	windowProps.Title = "FGE";
	windowProps.WindowMode = WindowMode::Windowed;
	
	auto app = new LauncherApp(windowProps);
	return app;
}