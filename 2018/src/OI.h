#pragma once
#include <Joystick.h>
#include <memory>

class OI
{
	public:
		OI();
		float getX(frc::Joystick::JoystickHand hand);
		float getY(frc::Joystick::JoystickHand hand);
	private:
		float removeDeadband(float joystickValue);
		frc::Joystick controller;

};
