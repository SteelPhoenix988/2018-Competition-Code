/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <string>

#include <Drive/DifferentialDrive.h>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <Spark.h>

/**
 * This is a demo program showing the use of the DifferentialDrive class.
 * The SampleRobot class is the base of a robot application that will
 * automatically call your Autonomous and OperatorControl methods at the right
 * time as controlled by the switches on the driver station or the field
 * controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're
 * inexperienced, don't. Unless you know what you are doing, complex code will
 * be much more difficult under this system. Use IterativeRobot or Command-Based
 * instead if you're new.
 */
using namespace std;
class Robot : public frc::SampleRobot {
public:
	Robot();

	void RobotInit() override;
	void Autonomous() override;
	void OperatorControl() override;
	void Test() override;

private:
	Joystick *controller;
	/*XboxController * controller2;*/
	SendableChooser<string> chooser;
	const string autoNameDefault = "Default";
	const string autoNameCustom = "My Auto";
	VictorSP *lf_motor, *rf_motor, *lr_motor, *rr_motor, *arm_motor, *r_intake_motor, *l_intake_motor;/*, *gear_a;*/
	/*Spark *gear_b;*/
	//To declare motor array and other motors ston the robot
	RobotDrive *myRobot;
		/*
		 * Runs during test mode
		 */
};
