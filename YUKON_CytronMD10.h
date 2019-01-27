#pragma once

class Adafruit_PWMServoDriver;
class Adafruit_MCP23017;

class YUKON_CytronMD10
{

  public:
	//SPECIAL,SPECIAL
	YUKON_CytronMD10(int channel, int pinpwm, int pindir, bool reversed);
	// //SPECIAL,GPIO
	// YUKON_CytronMD10(int channel, int pinpwm, int pindir, bool reversed);
	// //PWM,SPECIAL
	YUKON_CytronMD10(int channel, Adafruit_PWMServoDriver *Refpwm, int pindir, bool reversed);
	// YUKON_CytronMD10(int channel, int pinpwm, int pindir, bool reversed);
	// //PWM,GPIO
	YUKON_CytronMD10(int channel, Adafruit_PWMServoDriver *Refpwm, int pindir, Adafruit_MCP23017 *Refgpio, bool reversed);

	void Init();

	void SetMotorSpeed(float speed);

  private:
	Adafruit_PWMServoDriver *pwm;
	Adafruit_MCP23017 *gpio;

	bool _isPWMBoard = false;
	bool _isGPIOBoard = false;

	int dir = 0;

	int _pinPWM;
	int _pinDIR;
	bool _reverse;
	int _channel;

	float _prevSpeed = 1;
};
