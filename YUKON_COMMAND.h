#pragma once

#include <Arduino.h>

class YUKON_COMMAND
{

  public:
	YUKON_COMMAND();

	bool CmdIsRunning();
	uint8_t CmdPercentComplete();
	int16_t CmdSetPoint();

	bool CmdStart(String CmdName, long Timeout);
	bool CmdStart(String CmdName, int16_t StartPoint, int16_t SetPoint, long Timeout);
	void CmdDone();
	bool CmdTimedOut();
	bool CmdUpdatePercent(int16_t CurrentPoint);
	String CmdName();

  private:
	bool _CmdIsRunning = false;
	bool _CmdTimedOut = false;
	long _CmdTimeout = 0;
	uint8_t _CmdPercentComplete = 0;

	int16_t _CmdStartPoint = 0;
	int16_t _CmdSetPoint = 0;

	String _CmdName = "";
};
