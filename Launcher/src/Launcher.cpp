#include <Engine/EntryPoint.h>

#include "LauncherApp.h"


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