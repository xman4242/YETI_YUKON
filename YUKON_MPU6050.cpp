#include "YUKON_MPU6050.h"
#include <Arduino.h>

YUKON_MPU6050::YUKON_MPU6050()
{
}



void YUKON_MPU6050::Setup()
{
    Serial.println("GYRO SETUP");

    mpu.initialize();
    //pinMode(21, INPUT);
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(95);
    mpu.setYGyroOffset(62);
    mpu.setZGyroOffset(-3);
    mpu.setZAccelOffset(1539); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready

        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection

        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

void YUKON_MPU6050::Loop()
{
  
        // wait for MPU interrupt or extra packet(s) available
        // while (!MpuInterrupt && fifoCount < packetSize)
        // {
        // }

        // reset interrupt flag and get INT_STATUS byte
        MpuInterrupt = false;
        mpuIntStatus = mpu.getIntStatus();

        // get current FIFO count
        fifoCount = mpu.getFIFOCount();

        // check for overflow (this should never happen unless our code is too inefficient)
        if ((mpuIntStatus & 0x10) || fifoCount == 1024)
        {
            // reset so we can continue cleanly
            mpu.resetFIFO();
            Serial.println(F("FIFO overflow!"));

            // otherwise, check for DMP data ready interrupt (this should happen frequently)
        }
        else if (mpuIntStatus & 0x02)
        {
            // wait for correct available data length, should be a VERY short wait
            while (fifoCount < packetSize)
                fifoCount = mpu.getFIFOCount();

            // read a packet from FIFO
            mpu.getFIFOBytes(fifoBuffer, packetSize);

            // track FIFO count here in case there is > 1 packet available
            // (this lets us immediately read more without waiting for an interrupt)
            fifoCount -= packetSize;

            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            //Serial.print("ypr\t");
            // Serial.print(ypr[0] * 180 / M_PI);
            // Serial.print("\t");
            //Serial.print(ypr[1] * 180 / M_PI);
            //Serial.print("\t");
            //Serial.println(ypr[2] * 180 / M_PI);
        }

         UpdateHeading(ypr[0] * 180 / M_PI);
     

        // display.clearDisplay();
        // display.setTextSize(1);
        // display.setCursor(0, 0);

        // display.print(Robot.State.LeftEnc);
        // display.print(" ");
        // display.println(Robot.State.RightEnc);

        // display.print(Robot.State.LiftEnc);
        // display.print(" ");
        // display.println(Robot.State.Heading());

        // display.print("USB: ");
        // display.println(Robot.State.USBState);

        // display.print("Lift: ");
        // display.println(Robot.State.LiftSpeed);

        // display.display();

        //Lift.SetMotorSpeed(Robot.State.LiftSpeed);

        // delay(1);
    
}

void YUKON_MPU6050::UpdateHeading(float newHeading)
{
	float prev = _gyroDegrees;
	float dif = prev - newHeading;
	if (abs(dif) > 180)
	{
		if (dif < 0)
		{
			_gyroRotations--; 
		}
		else
		{
			_gyroRotations++;
		}
	}

	_gyroDegrees = newHeading;
}

float YUKON_MPU6050::Heading() {
	return _gyroDegrees + (360 * _gyroRotations);
}