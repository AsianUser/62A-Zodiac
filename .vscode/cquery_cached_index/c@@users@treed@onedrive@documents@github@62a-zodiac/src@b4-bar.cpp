#include "main.h"

pros::ADIDigitalOut RBMogo(2);
pros::ADIDigitalOut LBMogo(1);
pros::ADIDigitalOut Lock(3);
const int OUT = 0;
const int IN = 1;

void mogo(int position)
{
  if(position == OUT)
  { 
    RBMogo.set_value(true);
    LBMogo.set_value(true);
  }
  else if(position == IN)
  {
    RBMogo.set_value(false);
    LBMogo.set_value(false);
  }
}
