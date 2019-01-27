#pragma once

class Adafruit_PWMServoDriver;
class Adafruit_MCP23017;

class YUKON_DBH12V{
	
public:
	 YUKON_DBH12V(int pwm1, int pwm2, Adafruit_PWMServoDriver *Refpwm, bool reversed);

	void Init();

	void SetMotorSpeed(float speed);

private:

	Adafruit_PWMServoDriver *pwm;

	int _pwm1;
	int _pwm2;
	bool _reverse;

	float _prevSpeed = 1;
};


