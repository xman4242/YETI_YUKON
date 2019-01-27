#include "YUKON_COMMAND.h"

YUKON_COMMAND::YUKON_COMMAND()
{
}

bool YUKON_COMMAND::CmdIsRunning()
{
    return _CmdIsRunning;
}
String YUKON_COMMAND::CmdName()
{
    return _CmdName;
}
int16_t YUKON_COMMAND::CmdSetPoint()
{
    return _CmdSetPoint;
}
uint8_t YUKON_COMMAND::CmdPercentComplete()
{
    return _CmdPercentComplete;
}
bool YUKON_COMMAND::CmdStart(String CmdName, long Timeout)
{ 
    if (_CmdIsRunning)
        return false;

    _CmdName = CmdName;
    _CmdStartPoint = millis();
    _CmdSetPoint = Timeout + millis();
    _CmdIsRunning = true;
    _CmdTimeout = millis() + Timeout + 500;

    return true;
}

bool YUKON_COMMAND::CmdStart(String CmdName, int16_t StartPoint, int16_t SetPoint, long Timeout)
{ 
    if (_CmdIsRunning)
        return false;

    _CmdName = CmdName;
    _CmdStartPoint = StartPoint;
    _CmdSetPoint = SetPoint;
    _CmdIsRunning = true;
    _CmdTimeout = millis() + Timeout;
    _CmdPercentComplete = 0;

    return true;
}
void YUKON_COMMAND::CmdDone()
{
    _CmdName = "";
    _CmdStartPoint = 0;
    _CmdSetPoint = 0;
    _CmdIsRunning = false;
    _CmdTimeout = 0;
}
bool YUKON_COMMAND::CmdTimedOut()
{
    return _CmdTimedOut;
}

bool YUKON_COMMAND::CmdUpdatePercent(int16_t CurrentPoint)
{
     if(millis() > _CmdTimeout)
     {
        _CmdTimedOut = true;
        CmdDone();
        return false;
     }


     _CmdPercentComplete = ((CurrentPoint - _CmdStartPoint)*100) / (_CmdSetPoint - _CmdStartPoint);
     Serial.print("_CmdPercentComplete:");
     Serial.print(_CmdPercentComplete);
     Serial.print(" Var1:");
     Serial.print(((CurrentPoint - _CmdStartPoint)*100));
     Serial.print(" Var2:");
     Serial.println((_CmdSetPoint - _CmdStartPoint));
    //  Serial.print(" _CmdSetPoint:");
    //  Serial.println(_CmdSetPoint);

     if(_CmdPercentComplete >= 100)
     {
        _CmdPercentComplete = 100;
        CmdDone();
        return false;
     }

     return true;
}