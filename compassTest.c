
#pragma config(Sensor, S1,     rightTouchSensor, sensorTouch)
#pragma config(Sensor, S2,     leftTouchSensor, sensorTouch)
#pragma config(Sensor, S3,     lightSensor,    sensorLightActive)
#pragma config(Sensor, S4,     sonarSensor,    sensorSONAR)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// need to add another touch sensor when we find the dongle
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*--------------------------------------------------------------------------------------------------------*\
|*                                                                                                        *|
|*                                        - Multitasking  Demo -                                          *|
|*                                            ROBOTC on  NXT                                              *|
|*                                                                                                        *|
|*  This program is a very basic example of Multitasking using ROBOTC.  There are 3 Tasks: Main, TOne,    *|
|*  and TTwo.  Main starts tasks TOne and TTwo, running all 3 tasks simultaneously.  However, TTwo        *|
|*  doesn't do much until the Touch Sensor is activated.  Once activated, TOne will be terminated,        *|
|*  leaving Main and TTwo running simultaneously.  Once the Touch Sensor is deactivated, TTwo resumes     *|
|*  doing nothing, while Main and TOne run simultaneously again.  (This could be useful for running       *|
|*  certain display patterns in the background while 'real' code runs in the foreground, etc.)            *|
|*                                                                                                        *|
|*                                        ROBOT CONFIGURATION                                             *|
|*    NOTES:                                                                                              *|
|*    1)  For this demo, no motors are required, just one Touch Sensor in Sensor Port 2.                  *|
|*                                                                                                        *|
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
|*    Port 2                  touchSensor         Touch               NXT Touch Sensor                    *|
\*---------------------------------------------------------------------------------------------------4246-*/

//#include "drivers/hitechnic-touchmux.h"
//#include "drivers/LEGOTS-driver.h"

//C:\Program Files\Robomatter Inc\ROBOTC Development Environment\Drivers\rdpartyrobotcdr-3.3.1\drivers

// task TOne();    /* If Tasks are placed below Main in your code, list them all here like this above Main. */
// task TTwo();    /* Note** this is unnecessary for this code since the tasks are all above the Main Task. */

// Ken, Josh, Christie
// Robot 2

// task prototypes
task MoveTowardEgg;
task GrabEgg;
task DetectEgg;
task DetectWall;
task ResetArms;
task GoToScore;

// Constants

double DETECT_EGG_DISTANCE = 30;
double DETECT_EGG_CLOSE = 10;
double LIGHT_SENSOR_BLACK = 0;

double ARMS_OPEN = -60;
double ARMS_CLOSED = 15;
double ARMS_NORMAL = -5;

// how to get arms value:
//string display = nMotorEncoder[motorA];
// how to display to screen:
//nxtDisplayCenteredBigTextLine(3, display);     // Display that TOne is running simultaneously with Main.

// global vars

double homeLocation = 0; // store compass value of where nest direction is
double enemyLocation = 0; // compass value of enemy nest (opposite of home) - used to return to field maybe?
// don't need the hit vars?
bool leftTouchHit = false;
bool rightTouchHit = false;
bool detectedEgg = false;

bool eggInLeftHand = false;
bool eggInRightHand = false;

bool hasExitedForFirstTime = false;

bool goingToScore = false;


//==========================================================================================================================================

task Wander()
{
	double randDistance = 0;
	double randTurn = 0;
	int randDirection = 0;

	bool goingForward = false;

  while(true)
  {
    wait1Msec(300);                                 // Allow for a short wait, freeing up the CPU for other tasks..

    // go for longer to get out of base
    if (!hasExitedForFirstTime)
    {
    	ClearTimer(T1);
    	// go forward a large distance
    	while (time1[T1] < 400)
    	{
    		motor[motorC] = 75;
				motor[motorB] = 75;
    	}
    	hasExitedForFirstTime = true;
    }
    else
  	{
    	if (goingForward)
    	{
		    // go forward random distance (within a range, not too short, but not too long)
		    randDistance = rand() % 300;
		    ClearTimer(T1);
		    while (time1[T1] < randDistance)
		    {
		    	motor[motorC] = 75;
					motor[motorB] = 75;
		    }
		    goingForward = false;
	  	}
			else
			{
		    // calculate random number to turn (within a range, don't want to turn too small or too wide)
				// and rand direction
		    randTurn = rand() % 350;
		    randDirection = rand() % 2;
		    ClearTimer(T1);

		    // make the turn
		    while (time1[T1] < randTurn)
		    {
		    	if (randDirection)
		    	{
		    		motor[motorC] = 50;
						motor[motorB] = 0;
		    	}
		    	else
		    	{
		    		motor[motorC] = 0;
						motor[motorB] = 50;
		    	}
		    }
		    goingForward = true;
    	}
		}

    // repeat
  }
  return;
}

//==========================================================================================================================================

//==========================================================================================================================================

task DetectEgg()
{
  while(true)
  {
    wait1Msec(300);                                 // Allow for a short wait, freeing up the CPU for other tasks.

    if (SensorValue(sonarSensor) < DETECT_EGG_DISTANCE)
    {
    	detectedEgg = true;
    	StopTask(Wander);

    	// open arms
    	// how to get arms value:
			//string display = nMotorEncoder[motorA];
			// how to display to screen:
			//nxtDisplayCenteredBigTextLine(3, display);     // Display that TOne is running simultaneously with Main.
    	while (nMotorEncoder[motorA] > ARMS_OPEN)
    	{
    		motor[motorA] = -15;
    	}

    	motor[motorA] = 0;

    	StartTask(MoveTowardEgg);
    	break; // passed on to other task, should be restarted when appropriate
    }
  }
  return;
}

//==========================================================================================================================================

//==========================================================================================================================================

task MoveTowardEgg()
{
  while(true)
  {
    wait1Msec(300);                                 // Allow for a short wait, freeing up the CPU for other tasks.

    /*
    if (SensorValue(sonarSensor) < DETECT_EGG_CLOSE)
  	{
    	// move slowly towards egg
    }
    else if (SensorValue(sonarSensor) == DETECT_EGG_CLOSE)
    {
    	StartTask(GrabEgg);
    	break;
    }
    */

    while (SensorValue(sonarSensor) > DETECT_EGG_CLOSE)
    {
    	motor[motorC] = 20;
			motor[motorB] = 20;
    }

    motor[motorC] = 0;
		motor[motorB] = 0;

    StartTask(GrabEgg);
    break;
  }
  return;
}

//==========================================================================================================================================


//==========================================================================================================================================

task GrabEgg()
{
  while(true)
  {
    wait1Msec(300);                                 // Allow for a short wait, freeing up the CPU for other tasks.

    /*
    if (SensorValue(sonarSensor) < DETECT_EGG_CLOSE)
  	{
    	// move slowly towards egg
    }
    else if (SensorValue(sonarSensor) == DETECT_EGG_CLOSE)
    {
    	StartTask(GrabEgg);
    	break;
    }
    */

    ClearTimer(T3);
    eggInRightHand = false;

    if (!eggInRightHand)
    {
    	while (time1[T3] < 1200)
    	{
    		motor[motorC] = 0;
				motor[motorB] = 20;
			}
			eggInRightHand = true;
    }

    motor[motorC] = 0;
		motor[motorB] = 0;

    while (nMotorEncoder[motorA] < ARMS_CLOSED)
  	{
  		motor[motorA] = 15;
  	}

  	motor[motorA] = 0;

  	goingToScore = true;

    StartTask(GoToScore);
    break;

  }
  return;
}

//==========================================================================================================================================

//==========================================================================================================================================

task GoToScore()
{
  while(true)
  {
    wait1Msec(300);                                 // Allow for a short wait, freeing up the CPU for other tasks.

    /*
    if (SensorValue(sonarSensor) < DETECT_EGG_CLOSE)
  	{
    	// move slowly towards egg
    }
    else if (SensorValue(sonarSensor) == DETECT_EGG_CLOSE)
    {
    	StartTask(GrabEgg);
    	break;
    }
    */

    while (nMotorEncoder[motorA] < ARMS_CLOSED)
  	{
  		motor[motorA] = 15;
  	}

  }
  return;
}

//==========================================================================================================================================


//==========================================================================================================================================

// use T2 for wall avoid
task DetectWall()
{
  while(true)
  {
    wait1Msec(300);                                 // Allow for a short wait, freeing up the CPU for other tasks.

    if (SensorValue(leftTouchSensor) == 1)
    //if (TSreadState(LEFT_TOUCH))
    //if (HTTMUXisActive(HTTMUX, 1))
  	{
  		StopTask(Wander);

    	//leftTouchHit = true;
    	ClearTimer(T2);
    	while (time1[T2] < 300)
    	{
    		motor[motorC] = -75;
				motor[motorB] = -75;
    	}
    	ClearTimer(T2);
    	// turnRight
    	while (time1[T2] < 250)
    	{
    		motor[motorC] = 50;
				motor[motorB] = 0;
    	}
    	//leftTouchHit = false;
    	StartTask(Wander);
    }
    else if (SensorValue(rightTouchSensor) == 1)
    {
    	StopTask(Wander);

    	//rightTouchHit = true;
    	ClearTimer(T2);
    	while (time1[T2] < 300)
    	{
    		motor[motorC] = -75;
				motor[motorB] = -75;
    	}
    	ClearTimer(T2);
    	// turnLeft
    	while (time1[T2] < 250)
    	{
    		motor[motorC] = 0;
				motor[motorB] = 50;
    	}
    	StartTask(Wander);
    }
  }
  return;
}

//==========================================================================================================================================


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

task main()
{
  //StartTask(Wander);                             // Start Task TOne.
  StartTask(DetectEgg);                             // Start Task TTwo.
  //StartTask(DetectWall);

  while(true)
  {
    wait1Msec(300);                                 // Allow for a short wait, freeing up the CPU for other tasks.

    nxtDisplayCenteredBigTextLine(0, "TASK M");     // Display that Main is running.
  }
  return;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
