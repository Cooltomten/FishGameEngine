#pragma once

namespace FGE
{
	class Game
	{
	public:
		virtual ~Game() = default;

		virtual void OnStart() {}
		virtual void OnStop() {}
		virtual void OnUpdate(float aDeltaTime) {}
		virtual void OnRender() {}
	};
}