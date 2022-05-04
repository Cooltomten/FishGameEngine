#pragma once

#include <ds5w.h>
#include "Math/Vector2.hpp"


enum class DS5WButtons : unsigned short
{
	//Buttons X
	DpadUp			= 0x0001,
	DpadLeft		= 0x0002,
	DpadDown		= 0x0004,
	DpadRight		= 0x0008,

	Square			= 0x0010,
	Cross			= 0x0020,
	Circle			= 0x0040,
	Triangle		= 0x0080,

	//Buttons A
	LeftBumper		= 0x0101,
	RightBumper		= 0x0102,
	LeftTrigger		= 0x0104,
	RightTrigger	= 0x0108,

	Select			= 0x0110,
	Menu			= 0x0120,
	LeftStick		= 0x0140,
	RightStick		= 0x0180,

	//ButtonsB
	PlayStationLogo = 0x0201,
	TouchPad		= 0x0202,
	MicButton		= 0x0204
};

namespace CommonUtilities
{
	class DualSense5InputManager
	{
	public:
		DualSense5InputManager();
		~DualSense5InputManager();
		void Update();

		void SetTriggerResistanceNone(bool aLeftFlag);
		void SetTriggerResistanceSection(bool aLeftFlag, unsigned char aStartPos, unsigned char aEndPos);
		void SetTriggerResistanceContinuous(bool aLeftFlag, unsigned char aStartPos, unsigned char aForce);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="aStartPos">Position at witch the effect starts</param>
		/// <param name="aBeginForce">Force applied when trigger &gt;= (255 / 2)</param>
		/// <param name="aEndForce">Force applied when trigger is beyond 255</param>
		/// <param name="aFrequency">Vibration frequency of the trigger</param>
		/// <param name="aKeepEffect">Where the effect should keep playing when trigger goes beyond 255</param>
		/// <param name="aMiddleForce">Force applied when trigger &lt;= (255 / 2)</param>
		void SeTriggerResistanceExtended(bool aLeftFlag, unsigned char aStartPos, unsigned char aBeginForce,
			unsigned char aEndForce, unsigned char aFrequency, bool aKeepEffect, unsigned char aMiddleForce);

		bool GetButton(DS5WButtons aButton);

		CommonUtilities::Vector2f GetLeftStick();
		CommonUtilities::Vector2f GetRightStick();





	private:
		void FindController();
		//State object
		DS5W::DS5InputState myInState;
		DS5W::DS5OutputState myOutState;
		DS5W::DeviceContext myController;
		bool myControllerInitializedFlag;
		bool myControllerConnectedFlag;

	};
}