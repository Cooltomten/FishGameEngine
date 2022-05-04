#include "DualSense5InputManager.h"

#include <windows.h>

CommonUtilities::DualSense5InputManager::DualSense5InputManager()
{
	myControllerInitializedFlag = false;
	myControllerConnectedFlag = false;
	FindController();
}

CommonUtilities::DualSense5InputManager::~DualSense5InputManager()
{
	DS5W::freeDeviceContext(&myController);
}

void CommonUtilities::DualSense5InputManager::Update()
{
	if (myControllerInitializedFlag)
	{
		if (DS5W_SUCCESS(DS5W::getDeviceInputState(&myController, &myInState)))
		{
			DS5W::setDeviceOutputState(&myController, &myOutState);
		}
		else
		{
			myControllerConnectedFlag = false;
			if (DS5W_SUCCESS(DS5W::reconnectDevice(&myController)))
			{
				myControllerConnectedFlag = true;
			}
		}
	}
	else
	{
		FindController();
	}

}

void CommonUtilities::DualSense5InputManager::SetTriggerResistanceNone(bool aLeftFlag)
{
	DS5W::TriggerEffect& trigger = aLeftFlag ? myOutState.leftTriggerEffect : myOutState.rightTriggerEffect;

	trigger.effectType = DS5W::TriggerEffectType::NoResitance;
}

void CommonUtilities::DualSense5InputManager::SetTriggerResistanceSection(bool aLeftFlag, unsigned char aStartPos, unsigned char aEndPos)
{

	DS5W::TriggerEffect& trigger = aLeftFlag ? myOutState.leftTriggerEffect: myOutState.rightTriggerEffect;

	trigger.effectType = DS5W::TriggerEffectType::SectionResitance;
	trigger.Section.startPosition = aStartPos;
	trigger.Section.endPosition = aEndPos;
}

void CommonUtilities::DualSense5InputManager::SetTriggerResistanceContinuous(bool aLeftFlag, unsigned char aStartPos, unsigned char aForce)
{
	DS5W::TriggerEffect& trigger = aLeftFlag ? myOutState.leftTriggerEffect : myOutState.rightTriggerEffect;

	trigger.effectType = DS5W::TriggerEffectType::ContinuousResitance;
	trigger.Continuous.startPosition = aStartPos;
	trigger.Continuous.force = aForce;
}

void CommonUtilities::DualSense5InputManager::SeTriggerResistanceExtended(bool aLeftFlag, unsigned char aStartPos, unsigned char aBeginForce,
	unsigned char aEndForce,unsigned char aFrequency, bool aKeepEffect, unsigned char aMiddleForce)
{
	DS5W::TriggerEffect& trigger = aLeftFlag ? myOutState.leftTriggerEffect : myOutState.rightTriggerEffect;

	trigger.effectType = DS5W::TriggerEffectType::EffectEx;
	trigger.EffectEx.startPosition = aStartPos;
	trigger.EffectEx.beginForce = aBeginForce;
	trigger.EffectEx.endForce = aEndForce;
	trigger.EffectEx.frequency = aFrequency;
	trigger.EffectEx.keepEffect = aKeepEffect;
	trigger.EffectEx.middleForce = aMiddleForce;

}

bool CommonUtilities::DualSense5InputManager::GetButton(DS5WButtons aButton)
{
	if (!myControllerInitializedFlag || !myControllerConnectedFlag)
	{
		return false;
	}

	unsigned short input = static_cast<unsigned short>(aButton);

	if (input >= static_cast<unsigned short>(DS5WButtons::PlayStationLogo))
	{
		//Test Buttons b
		input = input ^ 0x0200;
		return myInState.buttonsB & input;
	}
	else if (input >= static_cast<unsigned short>(DS5WButtons::LeftBumper))
	{
		//Test buttons a
		input = input ^ 0x0100;
		return myInState.buttonsA & input;
	}
	else
	{
		//Test buttonsAndDpad
		return myInState.buttonsAndDpad & input;

	}
}

void CommonUtilities::DualSense5InputManager::FindController()
{
	//Enum all controllers presentf
	DS5W::DeviceEnumInfo infos[16];
	unsigned int controllersCount = 0;
	DS5W_ReturnValue rv = DS5W::enumDevices(infos, 16, &controllersCount);

	if (DS5W_SUCCESS(DS5W::initDeviceContext(&infos[0], &myController)))
	{
		//could connect controller
		ZeroMemory(&myInState, sizeof(DS5W::DS5InputState));
		ZeroMemory(&myOutState, sizeof(DS5W::DS5OutputState));
		myControllerInitializedFlag = true;
		myControllerConnectedFlag = true;
	}
	else
	{
		//failed to find a controller
	}
}
