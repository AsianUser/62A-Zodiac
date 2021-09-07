#include "main.h"

pros::Motor Spinner(3);

void spin(int power)
{
  Spinner.move(power);
}
void Spin_Control(void*)
{
  if(master.get_digital(DIGITAL_DOWN))
  {
    spin(127);
  }
  else if(master.get_digital(DIGITAL_A))
  {
    spin(-127);
  }
  else
  {
    spin(0);
  }
}
