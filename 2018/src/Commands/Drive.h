#pragma once

#include "../CommandBase.h"

class Drive: public CommandBase {
public:
	Drive();
	void Initialize() override;
	void Execute() override;
	bool IsFinished() override;
	void End() override;
	void Interrupted() override;
private:
	float xAxis;
	float yAxis;
	float rotAxis;

	void callMecanumDrive(float x, float y, float rot, float gyro);
};


