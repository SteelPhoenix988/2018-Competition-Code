#include <Joystick.h>
#include <math.h>
#include <memory>
#include "OI.h"
#include "RobotMap.h"
#include <WPILib.h>

OI::OI()
{
	controller(CONTROLLER_PORT);
}

float OI::getX(frc::Joystick::JoystickHand hand)
{
	float xValue = controller.GetX(hand);
	removeDeadband(xValue);
	return xValue;
}

float OI::getY(frc::Joystick::JoystickHand hand)
{
	float yValue = controller.GetX(hand);
	removeDeadband(yValue);
	return yValue;
}

float OI::removeDeadband(float joystickValue)
{
	return ( (fabs(static_cast<double>(joystickValue)) > 0.2f)?  joystickValue : 0.0f );
}
