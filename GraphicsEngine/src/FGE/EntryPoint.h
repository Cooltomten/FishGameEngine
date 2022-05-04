#pragma once

#include "FGE/Core/Application.h"

extern FGE::Application* FGE::CreateApplication();

namespace FGE
{
	int Main()
	{
		FGE::Application* pApp = FGE::CreateApplication();
		pApp->Run();
		delete pApp;
		return 0;
	}
}

int main()
{
	return FGE::Main();
}

