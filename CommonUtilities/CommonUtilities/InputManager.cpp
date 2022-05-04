#include "InputManager.h"
#include <iostream>
#include <bitset>
#include <windowsx.h>

namespace CommonUtilities
{


	InputManager::InputManager()
	{
		memset(myKeys, 0, sizeof(myKeys));
		memset(myPrevKeys, 0, sizeof(myKeys));
		memset(myCurrentKeys, 0, sizeof(myKeys));
		myCurrentMiddleMouseDelta = 0;
		myMiddleMouseDelta = 0;
		myMousePosDelta = { 0,0 };
	}
	bool InputManager::UpdateInput()
	{
		memcpy(myPrevKeys, myCurrentKeys, sizeof(myKeys));
		memcpy(myCurrentKeys, myKeys, sizeof(myKeys));
		
		
		myCurrentMiddleMouseDelta = myMiddleMouseDelta;
		myMiddleMouseDelta = 0;

		myMousePosDelta.x = myMousePos.x - myPrevMousePos.x;
		myMousePosDelta.y = myMousePos.y - myPrevMousePos.y;
		myPrevMousePos = myMousePos;
	
		return false;
	}
	bool InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		bool handledMessage = true;

		switch (message)
		{
		case WM_LBUTTONDOWN:
			myKeys[static_cast<int>(Keys::LeftMouse)] = 1;
			break;
		case WM_RBUTTONDOWN:
			myKeys[static_cast<int>(Keys::RightMouse)] = 1;
			break;
		case WM_MBUTTONDOWN:
			myKeys[static_cast<int>(Keys::MiddleMouse)] = 1;
			break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (myKeys[wParam] != 1)
			{
				myKeys[wParam] = 1;
			}
		}
		break;

		case WM_LBUTTONUP:
			myKeys[static_cast<int>(Keys::LeftMouse)] = 0;
			break;
		case WM_RBUTTONUP:
			myKeys[static_cast<int>(Keys::RightMouse)] = 0;
			break;
		case WM_MBUTTONUP:
			myKeys[static_cast<int>(Keys::MiddleMouse)] = 0;
			break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
			myKeys[wParam] = 0;
			break;

		case WM_MOUSEMOVE:
			myMousePos = MAKEPOINTS(lParam);
			break;

		case WM_MOUSEWHEEL:
			myMiddleMouseDelta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
			break;
		default:
			handledMessage = false;
			break;
		}

		return handledMessage;
	}

	bool InputManager::GetKeyDown(Keys aKey)
	{
		return (myCurrentKeys[static_cast<int>(aKey)] == 1) && (myPrevKeys[static_cast<int>(aKey)] == 0);
	}

	bool InputManager::GetKeyUp(Keys aKey)
	{
		return (myCurrentKeys[static_cast<int>(aKey)] == 0) && (myPrevKeys[static_cast<int>(aKey)] == 1);
	}

	bool InputManager::GetKey(Keys aKey)
	{
		return (myCurrentKeys[static_cast<int>(aKey)] == 1);
	}

	POINTS InputManager::GetMousePosition()
	{
		return myMousePos;
	}

	POINTS InputManager::GetMousePosDelta()
	{
		return myMousePosDelta;
	}

	float InputManager::GetMiddleMouseDelta()
	{
		return myCurrentMiddleMouseDelta;
	}

	void InputManager::SetMousePosition(short x, short y)
	{
		SetCursorPos(x, y);
	}

	void InputManager::SetLockMouseToWindow(bool aFlag)
	{
		if (aFlag)
		{
			HWND hWnd = GetActiveWindow();

			RECT rect;
			GetClientRect(hWnd, &rect);

			POINT ul;
			ul.x = rect.left;
			ul.y = rect.top;

			POINT lr;
			lr.x = rect.right;
			lr.y = rect.bottom;

			MapWindowPoints(hWnd, nullptr, &ul, 1);
			MapWindowPoints(hWnd, nullptr, &lr, 1);

			rect.left = ul.x;
			rect.top = ul.y;

			rect.right = lr.x;
			rect.bottom = lr.y;

			ClipCursor(&rect);
		}
		else
		{
			ClipCursor(NULL);
		}
	}

}