#include "main.h"

pros::ADIDigitalOut RBMogo(2);
pros::ADIDigitalOut RMogo2(5);
pros::ADIDigitalOut LBMogo(1);
pros::ADIDigitalOut LMogo2(4);
pros::ADIDigitalOut Lock(3);
const int OUT = 0;
const int IN = 1;
const int LOCK = 2;
const int UNLOCK = 3;

void Bmogo(int position)
{
  if(position == OUT)
  {
    RBMogo.set_value(true);
    LBMogo.set_value(true);
    LMogo2.set_value(true);
    RMogo2.set_value(true);
  }
  else if(position == IN)
  {
    RBMogo.set_value(false);
    RMogo2.set_value(false);
    LBMogo.set_value(false);
    LMogo2.set_value(false);

  }
}
void lock(int position)
{
  if(position == LOCK)
  {
    Lock.set_value(true);
  }
  else
  {
    Lock.set_value(false);
  }
}
int post = 0;
void Bmogo_Control(void*)
{
  while(true)
  {
  if(master.get_digital(DIGITAL_R2) && post == 0)
  {
    lock(LOCK);
    pros::delay(400);
    Bmogo(IN);
    post = 1;
  }
  else if(master.get_digital(DIGITAL_R2) && post == 1)
  {
    Bmogo(OUT);
    pros::delay(400);
    lock(UNLOCK);
    post = 0;
  }
  pros::delay(20);
  }
}
