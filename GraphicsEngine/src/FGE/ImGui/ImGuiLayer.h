#pragma once

namespace FGE
{
	class ImGuiLayer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();

		void Begin();
		void End();

		LRESULT UpdateEvents(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}