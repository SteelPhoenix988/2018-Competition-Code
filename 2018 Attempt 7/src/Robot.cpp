//#include <Encoder.h> Can't use encoder :(

#include <iostream>
#include <Joystick.h>
#include <memory>
#include <string>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <VictorSP.h>
#include <Robot.h>

using namespace std;

/*#define PI 3.14
#define TIRE_DIAMETER 25.4 // Tire diameter in centimers (10 in.)
#define PULSES_PER_REVOLUTION 1024 // Number of encoder pulses per revolution */

/**
 * This is a demo program showing the use of the RobotDrive class.
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

/*Notes
 * Xbox controller scheme for this project/intended purpose
 *Left stick to run the mecanum_drive
 *Right trigger to run the intake backwards
 *Right bumper to run the intake forwardss
 *Left trigger to run the spitter backwards
 *Hold left bumper to limit input
 * Key for prefixes
 * l = left
 * r = right/rear
 * f = front
 * Examples:
 * l_stick = left stick
 * rf = right front
 * lr = left rear
 * For VictorSP parameters (#s)
 * port numbers on roborio PWM
 * For GetRawAxis parameters (#s)
 *Corresponds to numbers in driver station
 *0:  Left X-axis
 *1:  Left Y-axis
 *2:  Left Trigger
 *3:  Right Trigger
 *4:  Right X-Axis
 *5:  Right Y-Axis
 *For GetRawButton parameters (#s)
 *1: A
 *2: B
 *3: X
 *4: Y
 *5: Left Bumper
 *6: Right Bumper
 *7: Back
 *8: Start
 *9: Left stick press
 *10: Right stick press
 *Also, if you need to find the documentation to find out what each function does/ what parameters it takes go to the bottom link
 * http://first.wpi.edu/FRC/roborio/release/docs/cpp/index.html
 * Added bookmark for link on Firefox (Other bookmarks -> WPILibC++ Main Page)
 *Important: You have to follow examples of joystick/robotdrive to be able to use functions. -> is how you dereference (get the value of at the address) to set certain characteristics of objects (OO Paradigm). It's a bit complicated.
 	Additionally, new is basically allocating memory somewhere in the computer architecture. This way objects can live past their scope and can even pass by reference.
 *Example: To use the xbox controller you have to do Joystick *name in the class and then name = new Joystick(parameters) in Robot() (the constructor where things are initialized)
 *Example: To use robot drive you have to do Robotdrive* name in public and then name = new RobotDrive(parameters)
 *Also important: Look at documentation (link above) if you don't know what parameters each function takes
 *Miscellaneous (in general)
 *Set is from -1 to 1 for PWM. 1 is full speed forward. -1 is full speed backwards. Basically motors work through these values, which fluctuates voltage (look up idiot's guide in bookmarks to learn more about electronics)
 *Diamater of tire = 10 inches.
 *drive_limiter is to prevent brownouts. Instead of multiplying to set input to a fixed percentage, the drive_limiter gradually increases every 1/2 second when input is detected and is compared to input to make sure that it gets to the desired speed without causing brownouts
 *C++ can be kind of confusing/overwhelming at first. Learn the fundamentals first (arrays, loops, conditionals, etc.), the advanced stuff (Object Oriented Paradigm/classes/pointers), and then the functions(look at documentation) to understand how to code
 *I found C++ e-textbooks that have structure and go over many of the topics needed to program (functions, pointers, classses, etc.). Starting with CPP is the best e-book for beginners.
 *Also, setting up can be a pain. Make sure all your include files (Project properties) and compiler settings are correct to fix possible problems.
 *Ask Kevin (leonidasIIV) on discord for possible help
 *In general, don't be afraid to ask for help. Go to Chief Delphi and ask on forums if needed
 */

Robot::Robot() {
	//Note SmartDashboard is not initialized here, wait until RobotInit to make SmartDashboard calls
	lf_motor = new VictorSP(3);
	lf_motor->SetInverted(false);
	//parameters are the ports for the motors
	rf_motor = new VictorSP(0);
	rf_motor -> SetInverted(false);
	//Inverting motors is part of the testing process. Right motors were set inverted after testing
	rr_motor = new VictorSP(1);
	rr_motor -> SetInverted(false);
	lr_motor = new VictorSP(2);
	lr_motor->SetInverted(false);
	arm_motor = new VictorSP(4);
	arm_motor ->SetInverted(true);
	r_intake_motor = new VictorSP(5);
	l_intake_motor = new VictorSP(6);
	/*gear_a = new VictorSP(6);
		gear_b = new Spark(7);*/
	myRobot = new RobotDrive(lf_motor,lr_motor,rf_motor,rr_motor);
	myRobot->SetExpiration(0.1);
	arm_motor->SetExpiration(0.1);
	//Safety reasons I think
	r_intake_motor->SetExpiration(0.1);
	l_intake_motor->SetExpiration(0.1);
	controller = new Joystick(0);
	/*deadBand Function is for mechanical reasons. The xbox controller joystick doesn't settle at 0.0 (for mechanical reasons). It can settle at something too.
		so, if the joystick is set at 0.0-0.2 /-0.2-0.0, the joystick value is set at 0. Otherwise, it returns normally.*/
}

void Robot::RobotInit() {
	chooser.AddDefault(autoNameDefault, autoNameDefault);
	chooser.AddObject(autoNameCustom, autoNameCustom);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);
	frc::SmartDashboard::PutNumber("RPM", 0);
}


/*
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable
 * chooser code works with the Java SmartDashboard. If you prefer the
 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
 * GetString line to get the auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the
 * SendableChooser make sure to add them to the chooser code above as well.
 */
void Robot::Autonomous() {
	auto autoSelected = chooser.GetSelected();
	// std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", autoNameDefault);
	cout << "Auto selected: " << autoSelected << std::endl;

	//		if (autoSelected == autoNameCustom)
	//		{
	// Custom Auto goes here
	cout << "Running custom Autonomous" << std::endl;
	myRobot->SetSafetyEnabled(false);
	myRobot->TankDrive(0.60, 0.60);
	frc::Wait(4.5);                // runs for 1/2 a second
	myRobot->TankDrive(0.0, 0.0);  // stops robot after waiting
	//		}
	//		else
	//		{
	//			// Default Auto goes here
	//			cout << "Running default Autonomous" << std::endl;
	//			myRobot->SetSafetyEnabled(false);
	//			myRobot->Drive(-0.5, 0.0); // drive forwards half speed
	//			frc::Wait(2.0);                // for 2 seconds
	//			myRobot->Drive(0.0, 0.0);  // stop robot
	//			/*
	//			 * Runs the motors with arcade steering.
	//			 */
	//		}
}


float deadBand(float joystickValue)
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

void Robot::OperatorControl() {
	double lftbump_limiter = 1.0;

	myRobot->SetSafetyEnabled(true);
	while (IsOperatorControl() && IsEnabled()) {
		//A-button
		if (controller->GetRawButton(5)==1)
		{
			lftbump_limiter = 0.75;
		}

		else
		{
			lftbump_limiter = 1.0;
		}

		myRobot->TankDrive
							(
									deadBand(controller->GetRawAxis(5))*lftbump_limiter,
									deadBand(controller->GetRawAxis(1))*lftbump_limiter
							);
		frc::Wait(0.005);
		//wait for a motor update time, so not all of the CPU of the computer is used. (Motors can be computer hogs)
	}
}
void Robot::Test(){}
START_ROBOT_CLASS(Robot)


/*

				//A-button
				if (controller->GetRawButton(5)==1)
				{
					lftbump_limiter = 0.75;
				}

				else
				{
					lftbump_limiter = 1.0;
				}

				/*myRobot->TankDrive
				(
						deadBand(controller->GetRawAxis(1))*lftbump_limiter,
						deadBand(controller->GetRawAxis(5))*lftbump_limiter
				);*/

// - is to flip the front side of the robot when driving.
/*myRobot->MecanumDrive_Cartesian(.5*-limit_input(deadBand(controller->GetRawAxis(0)), drive_limiter)*lftbump_limiter,
						.5*-limit_input(deadBand(controller->GetRawAxis(1)),drive_limiter)*lftbump_limiter,
						.5*limit_input(deadBand(controller->GetRawAxis(4)),drive_limiter)*lftbump_limiter);*/
//
//frc::SmartDashboard::PutNumber("RPM", myEncoder->GetRate());
/*
				if (controller->GetRawButton(5))
				{
					r_intake_motor->Set(1);
					l_intake_motor->Set(1);
				}
				else if(controller->GetRawButton(6))
				{
					r_intake_motor->Set(-1);
					l_intake_motor->Set(-1);
				}
				//left trigger
				if (deadBand(controller->GetRawAxis(2))>0.3)
				{
					arm_motor -> Set(-controller->GetRawAxis(2)*lftbump_limiter);
				}
				//right trigger
				else if (controller->GetRawButton(5))
				{
					arm_motor -> Set(controller->GetRawAxis(2)*lftbump_limiter);
				}
				else
				{
					arm_motor->Set(0);
					//So when they stop pressing the trigger, the motor stops
				}
 */
