#include "main.h"

const int GAIN = 0;
const int NEUTRAL = 0;
const int ALLIANCE = 434;

pros::Motor TwoBar(15, MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES);
int Get_pos()
{
  return TwoBar.get_position();
}
int  get_Bar_vel()
{
  return TwoBar.get_actual_velocity();
}

void Bar(int input)
{
  TwoBar = input;
}
void Position(int position)
{
  if(position == Gain)
  {
    //if(TwoBar >= GAIN)
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
int timer = 0;
bool hold = false;
bool previous_hold = false;
void Two_Bar_Control(void*)
{
  while(true)
  {
  if(master.get_digital(DIGITAL_L2) && position == 0 /*&& (position == 1 || Full_Rotation)*/)
  {
    hold = true;

  /*  Position(Alliance);
    position = 2;
    if(Full_Rotation)
    {
      Full_Rotation = false;
    }*/
    Bar(127);


  }
  else if(master.get_digital(DIGITAL_L2) && position == 1 /*&& position == 0 && !Full_Rotation*/)
  {
    hold = true;
    //Position(Neutral);
    //position = 1;
    Bar(-127);
  }
  else if(position == 1 && !master.get_digital(DIGITAL_L2))
  {
    hold = false;
    Bar(-20);

  }

  else
  {
    hold = false;
    Bar(0);
  }
  if(hold != previous_hold && previous_hold == true)
  {
    if(position == 0)
    position = 1;
    else
    position = 0;
  }
  printf("Hold: %d \n", hold);
  printf("Prev Hold: %d \n", previous_hold);
  printf("position: %d \n", position);
  /*if(master.get_digital(DIGITAL_L2) && position == 2 && !Full_Rotation)
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
  }*/
  previous_hold = hold;
  pros::delay(20);
  }
}
