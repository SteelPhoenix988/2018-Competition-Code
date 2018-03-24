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
/*
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
class Robot: public frc::SampleRobot {
private:
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

	enum DriveTrainSide
	{
		LEFT,
		RIGHT
	};
	Joystick *controller;
	SendableChooser<string> chooser;
	const string autoNameDefault = "Default";
	const string autoNameCustom = "My Auto";
	VictorSP *leftFrontMotor, *rightFrontMotor, *leftRearMotor, *rightRearMotor, *arm_motor, *r_intake_motor, *l_intake_motor;/*, *gear_a;*/
	/*Spark *gear_b;*/
	//To declare motor array and other motors on the robot
	RobotDrive *myRobot;
	bool inversionState = false;
		void Test() override {

		}
		/*
		 * Runs during test mode
		 */

public:
	void setAllInverted(bool flag)
	{
		leftFrontMotor->SetInverted(flag);
		leftRearMotor->SetInverted(flag);
		rightFrontMotor->SetInverted(flag);
		rightRearMotor->SetInverted(flag);
	}
	Robot() {
		//Note SmartDashboard is not initialized here, wait until RobotInit to make SmartDashboard calls
		leftFrontMotor = new VictorSP(3);
		leftFrontMotor->SetInverted(inversionState);
		leftRearMotor = new VictorSP(2);
		leftRearMotor->SetInverted(inversionState);

		rightFrontMotor = new VictorSP(0);
		rightFrontMotor -> SetInverted(inversionState);
		rightRearMotor = new VictorSP(1);
		rightRearMotor -> SetInverted(inversionState);

		myRobot = new RobotDrive( leftFrontMotor,leftRearMotor,rightFrontMotor,rightRearMotor );
		myRobot->SetExpiration(0.1);

		controller = new Joystick(0);
	}

	void RobotInit() {
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
	void Autonomous() {
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

	bool getButton(Button button)
	{
		return controller->GetRawButton(static_cast<int>(button));
	}

	double getAxis(Hand hand, Axis axis)
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

	bool setInversion(DriveTrainSide side, bool flag )
	{
		switch(side)
		{
			case LEFT:
				leftFrontMotor->SetInverted(flag);
				leftRearMotor->SetInverted(flag);
			break;
				rightFrontMotor->SetInverted(flag);
				rightRearMotor->SetInverted(flag);
		}
	}
	void OperatorControl() override {
		double lftbump_limiter = 1.0;

		myRobot->SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled()) {


				//A-button
				if (getButton(A))
				{
					lftbump_limiter = 0.75;
				}

				else
				{
					lftbump_limiter = 1.0;
				}

				if(getAxis(LEFT, Y_AXIS) == 0)
				{
					if(getAxis(RIGHT, X_AXIS) > 0)
					{
						setInversion(RIGHT, true);
					}
					else if (getAxis(RIGHT, X_AXIS) < 0)
					{

					}
					myRobot->TankDrive( getAxis(RIGHT, X_AXIS), getAxis(RIGHT, Y_AXIS))
				}
				else if(getAxis(RIGHT, X_AXIS) == 0)
				{
					myRobot->TankDrive( getAxis(LEFT, Y_AXIS), getAxis(LEFT, Y_AXIS));
				}
				frc::Wait(0.005);
				//wait for a motor update time, so not all of the CPU of the computer is used. (Motors can be computer hogs)
				}
		}


	};
START_ROBOT_CLASS(Robot)
