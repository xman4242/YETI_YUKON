
#include "YUKON_DBH12V.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23017.h>

YUKON_DBH12V::YUKON_DBH12V(int pwm1, int pwm2, Adafruit_PWMServoDriver *Refpwm, bool reversed)
{
	_pwm1 = pwm1;
	_pwm2 = pwm2;
	_reverse = reversed;
	pwm = Refpwm;
}

void YUKON_DBH12V::Init()
{
	pwm->setPWM(_pwm1, 0, 0);
	pwm->setPWM(_pwm2, 0, 0);
}

void YUKON_DBH12V::SetMotorSpeed(float speed)
{
	if (_reverse)
		speed = speed * -1;

	if (_prevSpeed != speed)
	{
		_prevSpeed = speed;

		if (speed > 0)
		{
			pwm->setPWM(_pwm1, 0, 0);
			pwm->setPWM(_pwm2, 0, abs(map(speed, 0, 255, 0, 4095)));
		}
		else
		{
			pwm->setPWM(_pwm1, 0, abs(map(speed, 0, 255, 0, 4095)));
			pwm->setPWM(_pwm2, 0, 0);
		}
	}
}
