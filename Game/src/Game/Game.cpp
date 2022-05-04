#include "Game.h"
void Game::OnStart()
{
}

void Game::OnStop()
{
}

void Game::OnUpdate(float aDeltaTime)
{

}

void Game::OnRender()
{
}

FGE::Game* CreateGame()
{
	return new Game();
}