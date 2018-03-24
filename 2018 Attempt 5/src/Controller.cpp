/*
 * Controller.cpp
 *
 *  Created on: Mar 24, 2018
 *      Author: FRCrobotics988
 */

#include "Controller.h"
#include <math.h>

Controller::Controller(const int port) {

	controller = std::make_unique<frc::Joystick>(port);
}

bool Controller::getButton(Button button)
{
	return controller->GetRawButton(static_cast<int>(button));
}

double Controller::getAxis(Hand hand, Axis axis)
{
	switch(hand)
	{
		case LEFT:
			switch(axis)
			{
				case X_AXIS:
					return deadBand(controller->GetRawAxis(0));
				break;

				case Y_AXIS:
					return deadBand(controller->GetRawAxis(1));
				break;

				case TRIGGER:
					return deadBand(controller->GetRawAxis(2));
				break;
			}
		break;

		case RIGHT:
			switch(axis)
			{
				case TRIGGER:
					return deadBand(controller->GetRawAxis(3));
				break;

				case X_AXIS:
					return deadBand(controller->GetRawAxis(4));
				break;

				case Y_AXIS:
					return deadBand(controller->GetRawAxis(5));
				break;
			}
		break;
	}
}

float Controller::deadBand(float joystickValue)
{
	if(std::fabs(joystickValue) > 0.2f)
				{
					return joystickValue;
				}
				else
				{
					return 0.0f;
				}
}
