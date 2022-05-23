#pragma once
#include "ComponentSystem/"
#include <FGE/Core/Game.h>

class Game : public FGE::Game
{
public:
	void OnStart() override;
	void OnStop() override;

	void OnUpdate(float aDeltaTime) override;
	void OnRender() override;

private:
	
};
