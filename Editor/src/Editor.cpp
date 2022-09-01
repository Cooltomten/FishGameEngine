#include <Engine/EntryPoint.h>

#include "EditorApp.h"


FGE::Application* FGE::CreateApplication()
{
	FGE::WindowProperties windowProps;
	windowProps.X = 0;
	windowProps.Y = 0;
	windowProps.Width = 1280;
	windowProps.Height = 720;
	windowProps.IsVSync = false;
	windowProps.Title = "FGE";
	windowProps.WindowMode = WindowMode::Windowed;

	auto app = new EditorApp(windowProps);
	return app;
}