#include <Commands/Drive.h>

Drive::Drive() :  xAxis(0), yAxis(0), rotAxis(0)
{
	Requires(CommandBase::exampleSubsystem.get());
}

// Called just before this Command runs the first time
void Drive::Initialize()
{
	xAxis = 0;
	yAxis = 0;
	rotAxis = 0;
}

// Called repeatedly when this Command is scheduled to run
void Drive::Execute()
{
	xAxis = CommandBase::oi->getX(frc::Joystick::kRightHand);
	yAxis = CommandBase::oi->getY(frc::Joystick::kLeftHand);
	rotAxis = CommandBase::oi->getX(frc::Joystick::kLeftHand);
	Drive::callMecanumDrive(xAxis, yAxis, rotAxis);
}

// Make this return true when this Command no longer needs to run execute()
bool Drive::IsFinished()
{
	return false;
}

// Called once after isFinished returns true
void Drive::End()
{

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Drive::Interrupted()
{

}

void Drive::callMecanumDrive(float x, float y, float rot, float gyro)
{
	//CommandBase::mecanumDrive->DriveCartesian(x, y, rot, gyro);
}

