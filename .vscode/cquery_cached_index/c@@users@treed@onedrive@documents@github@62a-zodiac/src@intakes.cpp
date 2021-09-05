#include "main.h"

pros::Motor Intake(4);

void intake(int power)
{
  Intake.move(power);
}
void Intake_Control(void*)
{
  while(true)
  {
  if(master.get_digital(DIGITAL_L1))
  {
     intake(127);
  }
  else if(master.get_digital(DIGITAL_DOWN))
  {
    intake(-127);
  }
  pros::delay(20);
  }
}
