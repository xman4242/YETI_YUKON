#include "YUKON_AUTON.h"

YUKON_AUTON::YUKON_AUTON()
{

}

void YUKON_AUTON::Setup()
{
     preferences.begin("auton", false);
    unsigned int aNum = preferences.getUInt("lockednum", 0);
    preferences.putUInt("lockednum", 0);
    preferences.end();

    if(aNum > 0)
    {
        _QueuedProgramNumber = aNum;
        _QueuedAuton = "Auton" + String(_QueuedProgramNumber);

        LaunchQueued();
    }
}

void YUKON_AUTON::StartAuton(String AutonName)
{
    _RunningAuton = AutonName;
}

void YUKON_AUTON::LaunchQueued()
{
    if (_QueuedAuton != "")
    {
        _RunningAuton = _QueuedAuton;
        _QueuedAuton = "";
        _AutonARMED = false;
    }
}
bool YUKON_AUTON::IsArmed()
{
    return _AutonARMED;
}
bool YUKON_AUTON::IsArmLocked()
{
    return _AutonARMLOCKED;
}
String YUKON_AUTON::QueuedProgramName()
{
    return _QueuedAuton;
}

void YUKON_AUTON::QueueNext()
{
    if (_QueuedProgramNumber == _MaxProgramNumber)
    {
        _QueuedProgramNumber = 0;
        _QueuedAuton = "";
    }
    else
    {
        _QueuedProgramNumber++;
        _QueuedAuton = "Auton" + String(_QueuedProgramNumber);
    }
}
void YUKON_AUTON::QueuePrev()
{
    if (_QueuedProgramNumber == 0)
    {
        _QueuedProgramNumber = _MaxProgramNumber;
        _QueuedAuton = "Auton" + String(_MaxProgramNumber);
    }
    else
    {
        _QueuedProgramNumber--;
        if (_QueuedProgramNumber > 0)
            _QueuedAuton = "Auton" + String(_QueuedProgramNumber);
        else
            _QueuedAuton = "";
    }
}
void YUKON_AUTON::ToggleArmed()
{
    _AutonARMED = !_AutonARMED;
}

void YUKON_AUTON::ToggleLockArmed()
{
    _AutonARMLOCKED = !_AutonARMLOCKED;

    preferences.begin("auton", false);

    if(_AutonARMLOCKED)
        preferences.putUInt("lockednum", _QueuedProgramNumber);
    else
        preferences.putUInt("lockednum", 0);

    preferences.end();

    //unsigned int counter = preferences.getUInt("lockednum", 0);   
   // counter++;
    //preferences.putUInt("lockednum", counter);

    //ESP.restart();
}