#include "main.h"

const int GAIN = 0;
const int NEUTRAL = 0;
const int ALLIANCE = 0;

pros::Motor TwoBar(15, MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES);

void Bar(int input)
{
  TwoBar = input;
}
void Position(int position)
{
  if(position == Gain)
  {
    Bar(GAIN);
  }
  else if(position == Neutral)
  {
    Bar(NEUTRAL);
  }
  else if(position == Alliance)
  {
    Bar(ALLIANCE);
  }
}
int position = 0;
int Two_Bar_Timer = 0;
bool Full_Rotation = false;
void Two_Bar_Control(void*)
{
  while(true)
  {
  if(master.get_digital(DIGITAL_L2) && (position == 1 || Full_Rotation))
  {
    Position(Alliance);
    position = 2;
    if(Full_Rotation)
    {
      Full_Rotation = false;
    }
  }
  if(master.get_digital(DIGITAL_L2) && position == 0 && !Full_Rotation)
  {
    Position(Neutral);
    position = 1;

  }
  if(master.get_digital(DIGITAL_L2) && position == 2 && !Full_Rotation)
  {
    Position(Gain);
    position = 0;
  }
  if(master.get_digital(DIGITAL_L2))
  {
    if(position == 0 || position == 1)
    {
      Two_Bar_Timer += 1;
      if(Two_Bar_Timer >= 100)
      {
        Full_Rotation = true;
      }
    }
  }
  pros::delay(20);
  }
}
