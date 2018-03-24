#pragma once
#include <Joystick.h>
#include <memory>
class Controller {
public:
	enum Hand
	{
		LEFT,
		RIGHT
	};
	enum Axis
	{
		X_AXIS,
		Y_AXIS,
		TRIGGER
	};
	enum Button
	{
		A=1,
		B,
		X,
		Y,
		LEFT_BUMPER,
		RIGHT_BUMPER,
		BACK,
		START,
		LEFT_STICK_PRESS,
		RIGHT_STICK_PRESS
	};
	Controller(const int port);
	double getAxis(enum Hand, enum Axis);
	bool getButton(enum Button);
private:
	std::unique_ptr<frc::Joystick> controller;
	float deadBand(float joystickValue);
};
