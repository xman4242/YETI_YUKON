#ifndef _YETI_YUKON_H_
#define _YETI_YUKON_H_

//WIFI and Wireless Deploy Libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ArduinoOTA.h>

//OLED Display Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_PWMServoDriver.h> //PWM Library
#include <Adafruit_MCP23017.h>       //GPIO Library
#include <Adafruit_MCP3008.h>        //ADC Library

#include "YUKON_MPU6050.h"

class YETI_YUKON
{

public:
  YETI_YUKON(const char *RobotName, const char *Password);

  Adafruit_SSD1306 OLED;

  Adafruit_PWMServoDriver PWM;
  Adafruit_MCP23017 GPIO;
  Adafruit_MCP3008 ADC;

  AsyncWebServer server;
  DNSServer dns;

  YUKON_MPU6050 GYRO;

  void Setup();
  void Loop();
  void WatchdogLoop();
  void SetupWIFI();

  void EnableWatchdog();
  void DisableWatchdog();

  //Value Mappers
  int16_t JoystickTo255(int16_t JoystickValue, int16_t Deadzone);
  int16_t XBOXJoystickTo255(int16_t JoystickValue, int16_t Deadzone);
  int16_t ScrubInputWithParameters(int16_t JoystickValue, int16_t Deadzone, int16_t InputMin, int16_t InputMax, bool reverseInput);

private:
  void SetupOTA();
  const char *robotName = "";
  const char *password = "";

  volatile long _lastWatchdogPat = 0;
  volatile long _watchdogBite = 1000;
  volatile bool _watchdogEnabled = false;
  volatile bool _watchdogPaused = false;
};

#endif /* _YETI_YUKON_H_ */