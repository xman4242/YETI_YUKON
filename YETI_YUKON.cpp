#include "YETI_YUKON.h"

YETI_YUKON::YETI_YUKON(const char *RobotName, const char *Password) : OLED(16), server(80)
{
    robotName = RobotName;
    password = Password;
}

void YETI_YUKON::Setup()
{
    GYRO.Setup();

    PWM.begin();
    PWM.setPWMFreq(500);

    GPIO.begin(); // use default address 0

    ADC.begin(2);

    //Initialize the Display
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.clearDisplay();
    OLED.setCursor(0, 0);

    OLED.setTextSize(1);
    OLED.setTextColor(WHITE);
    OLED.display();

    pinMode(25, OUTPUT);
    digitalWrite(25, LOW);

    _lastWatchdogPat = millis();

    //delay(1000); //make sure the IP address stays on the screen for 1 second.
    //Should be replaced later with something else.

    //SetupWIFI();
}

void YETI_YUKON::SetupWIFI()
{
    _watchdogPaused = true;
    _lastWatchdogPat = millis();

    AsyncWiFiManager wifiManager(&server, &dns);
    //wifiManager.resetSettings();
    wifiManager.autoConnect();
    SetupOTA();

    OLED.clearDisplay();
    OLED.setCursor(0, 0);
    OLED.println(WiFi.localIP());
    OLED.println(robotName);
    OLED.display();

    delay(1000);

    _lastWatchdogPat = millis();
    _watchdogPaused = false;
}

void YETI_YUKON::Loop()
{
    GYRO.Loop();
    // OLED.clearDisplay();
    // OLED.setCursor(0, 0);
    // OLED.println(GYRO.Heading());
    // OLED.display();

    ArduinoOTA.handle();

    _lastWatchdogPat = millis();

    //delay(1);
}

void YETI_YUKON::EnableWatchdog()
{
    _lastWatchdogPat = millis();
    _watchdogEnabled = true;
}
void YETI_YUKON::DisableWatchdog()
{
    _watchdogEnabled = false;
}

void YETI_YUKON::WatchdogLoop()
{
    if (_watchdogEnabled && !_watchdogPaused && millis() - _lastWatchdogPat > _watchdogBite)
    {
        Serial.println("Watchdog Bite! Restarting.");
        Serial.print(millis() - _lastWatchdogPat);
        Serial.print(" > ");
        Serial.println(_watchdogBite);
        ESP.restart();
    }
}

int PrevPercent = 0;
unsigned long PrevMillis = 0;
unsigned long LastUpdate = 0;
void YETI_YUKON::SetupOTA()
{
    ArduinoOTA.setHostname(robotName);
    ArduinoOTA.setPassword(password);
    ArduinoOTA
        .onStart([this]() {
            DisableWatchdog();

            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            PrevMillis = millis();
            LastUpdate = millis();

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            //KillAllTasks();
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([this](unsigned int progress, unsigned int total) {
            if (millis() - LastUpdate > 1000)
            {
                LastUpdate = millis();
                if (PrevPercent < (progress / (total / 100)))
                {
                    PrevPercent = (progress / (total / 100));
                    //disp.WritePercentLarge(PrevPercent, 0, 0);
                    OLED.clearDisplay();
                    OLED.setTextSize(2);
                    OLED.setCursor(0, 0);
                    OLED.print(PrevPercent);
                    OLED.print("%    ");
                    OLED.print((millis() - PrevMillis) / 1000);
                    OLED.println("s");

                    for (int i = 0; i < PrevPercent; i += 11)
                    {
                        OLED.print("=");
                    }

                    OLED.display();
                }
            }
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.begin();
}

int16_t YETI_YUKON::ScrubInputWithParameters(int16_t JoystickValue, int16_t Deadzone, int16_t InputMin, int16_t InputMax, bool reverseInput)
{
    int16_t _inZero = abs(InputMax + InputMin) / 2;

    if (JoystickValue > _inZero + Deadzone)
    {
        JoystickValue = map(JoystickValue, _inZero + Deadzone, InputMax, 0, 255);
    }
    else if (JoystickValue < _inZero - Deadzone)
    {
        JoystickValue = map(JoystickValue, _inZero - Deadzone, InputMin, 0, -255);
    }
    else
    {
        JoystickValue = 0;
    }
    if (reverseInput)
        return JoystickValue * -1;

    return JoystickValue;
}

//Value Mappers
int16_t YETI_YUKON::JoystickTo255(int16_t JoystickValue, int16_t Deadzone)
{
    return ScrubInputWithParameters(JoystickValue, Deadzone, 0, 255, true);
}
int16_t YETI_YUKON::XBOXJoystickTo255(int16_t JoystickValue, int16_t Deadzone)
{
    return ScrubInputWithParameters(JoystickValue, Deadzone, -32767, 32767, false);
}