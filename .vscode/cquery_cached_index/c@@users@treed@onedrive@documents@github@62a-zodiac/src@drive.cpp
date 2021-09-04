#include "main.h"
pros::Controller master(CONTROLLER_MASTER);

pros::Motor left_back(14, true);
pros::Motor left_front(13, true);
pros::Motor right_back(15);
pros::Motor right_front(16);
pros::Imu Inertial(20);
pros::ADILineSensor RightLineSensor(3);
pros::ADILineSensor LeftLineSensor(1);

float wheel_size = 3.25; //2.75 // 6.5
float tick_per_rev = 535 * (5/3); //360
float circumference = wheel_size*M_PI;
float tick_per_inch = (tick_per_rev/circumference);

void RightTurn(int degrees, int power)
{
  if(RightEncoderval() >= degrees)
  {
    drive(power, -power);
  }
}
//void LeftTurn(int degrees, int power)
//{
//  if(RightEncoderval() >= degrees)
  //{
  //  drive(power, power);
  //}
//}

void motorbreak()
{
  left_back.set_brake_mode(MOTOR_BRAKE_BRAKE);
  left_front.set_brake_mode(MOTOR_BRAKE_BRAKE);
  right_back.set_brake_mode(MOTOR_BRAKE_BRAKE);
  right_front.set_brake_mode(MOTOR_BRAKE_BRAKE);
}
void motorcoast()
{
  left_back.set_brake_mode(MOTOR_BRAKE_COAST);
  left_front.set_brake_mode(MOTOR_BRAKE_COAST);
  right_back.set_brake_mode(MOTOR_BRAKE_COAST);
  right_front.set_brake_mode(MOTOR_BRAKE_COAST);
}
int clipit(float max, float min, float input) {
  if (input > max)    return max;
  else if (input < min)
    return min;
  return input;
}

int velocityRight()
{
  return right_front.get_actual_velocity();
}
int velocityLeft()
{
  return left_front.get_actual_velocity();
}
bool checkshift()
{
  return master.get_digital(DIGITAL_R2);
}
float InertialReadings ()
{
  return Inertial.get_rotation();
}
void ResetInertial ()
{
  Inertial.reset();
  pros::delay(2000);
}
int RightEncoderval ()
{
  /*
  int RightAvrage = (right_front.get_position() + right_back.get_position()) / 2;
  return RightAvrage;
  */
  return right_back.get_position();

}
int LeftEncoderval ()
{
  /*
  int RightAvrage = (left_front.get_position() + left_back.get_position()) / 2;
  return RightAvrage;
  */
  return left_back.get_position();


}
void Inertialtest()
{
  while(true)
  {
    printf("Get Rotation: %f \n", Inertial.get_rotation());
    printf("Get Heading: %f \n", Inertial.get_heading());
  }
}
void drive(int r, int l)
{
  right_front.move(r);
  right_back.move(r);
  left_back.move(l);
  left_front.move(l);

}


int Right_Target_Value;
int Left_Target_Value;
int currentdrivetype;
 int angle = 0;
 int maxval = 80;
 int minval = -110;
 int RMax;
 int LMax;
 int RightArc;
 int LeftArc;
 //THE BELOW IS NOT TURN KP
 float Kp = 0.19; //0.035  //THIS IS DRIVE KP
 //THE ABOVE IS NOT TURN KP

//DON'T BE AN IDIOT AND MAKE SURE THAT YOUR INERSIAL PORT IS CORRECT!!!!

//THIS IS FOR DRIVING FOWARD
 float Kd = 0.0;//40.0 20.0
 //DON"T BE AN IDIOT AND THINK THAT WHAT IS ABOVE IS FOR TURNING
 // BELOW IS FOR TURNING!!!
  float Heading_Kd = 50.0; //THIS IS FOR TURNING NOT DRIVING!!!
  float Ki = 0.27;//0.001 0.2
  float start_i = 10;
//_____________________________________________________________________________________________________________
void
Drive_pid(void*)
 {
   float Re;
   float Le;
   float Angle_error;
   int Rightmotorval;
   int Leftmotorval;
   int GOutput;
   float HeadingKp = 9;
   float Gyro_Kp = 6.75;//1.05
   float integral;
   float derivitive;
   float Rderivitive;
   float Lderivitive;
   float prev_error;
   float Rprev_error;
   float Lprev_error;
   float incherror;
   int Rprev_line;
   int Rline_error;
   int Lprev_line;
   int Lline_error;
   int ROutput;
   int LOutput;

  while (true)
   {
     Angle_error = angle - InertialReadings();
     //printf("Inertial readings: %f", InertialReadings());
     Le = Left_Target_Value - LeftEncoderval();
     Re = Right_Target_Value - RightEncoderval();
      // printf("Angle_error: %d \n", Angle_error);
    //printf("error = %f \n", Re);
    //printf("Encoder Value = %d \n", RightEncoderval());
    //printf("Target Value = %d \n", Right_Target_Value);
     if(fabs(Angle_error) < start_i)
        integral = integral + Angle_error;
     else
       integral = 0;
       if(currentdrivetype == Drive)
       {
         Rderivitive = Re - Rprev_error;
         Rprev_error = Re;
         Lderivitive = Le - Lprev_error;
         Lprev_error = Le;
       }
       else if(currentdrivetype == Turn)
       {
         derivitive = Angle_error - prev_error;
         prev_error = Angle_error;
       }
     Rightmotorval = Kp * Re;
     Leftmotorval = Kp * Le;
     if(currentdrivetype == Drive)
       GOutput = Angle_error * HeadingKp;
     else if(currentdrivetype == Turn || currentdrivetype == RSwing || currentdrivetype == LSwing || currentdrivetype == Arc)
       GOutput = Angle_error * Gyro_Kp;
      // printf("Angle_error: %d \n", Angle_error);
      //printf("Rightmotorval = %d \n", Rightmotorval);
      Rightmotorval = clipit(maxval, minval, Rightmotorval);
      Leftmotorval = clipit(maxval, minval, Leftmotorval);
     //printf("Rightmotorval = %d \n", Rightmotorval);
    // printf("Angle_P = %d and InertialReadings = %d \n", Angle_P, InertialReadings());
      if (pros::millis()>=2000)
      {
      //  printf("working \n");
        if(currentdrivetype == Turn)
        {
         GOutput = clipit(maxval, minval, GOutput);
          //printf("Angle error: %f \n", Angle_error);
          drive(-((Angle_error*Gyro_Kp) + (Ki *integral) + (Heading_Kd * derivitive)), ((Angle_error*Gyro_Kp) + (Ki *integral) + (Heading_Kd * derivitive)));
        }
        else if(currentdrivetype == Drive)
        {
          drive(Rightmotorval - GOutput + (Kd * Rderivitive), Leftmotorval + GOutput + (Kd * Lderivitive));
          //drive(Rightmotorval, Leftmotorval);
          //printf("Angle error: %f \n", Angle_error);
          //printf("GOutput: %d \n", GOutput);
        }
        else if(currentdrivetype == Line)
        {
          drive(Rightmotorval + ROutput, Leftmotorval + LOutput);
        }
        else if(currentdrivetype == RSwing)
        {
          GOutput = clipit(maxval, minval, GOutput);
          drive(-GOutput, 0);
        }
        else if(currentdrivetype == LSwing)
        {
          GOutput = clipit(maxval, minval, GOutput);
          drive(0, GOutput);
        }
      }
      else if(currentdrivetype == Arc)
      {
        RightArc = clipit(RMax, minval, GOutput);
        LeftArc = clipit(LMax, minval, GOutput);
        drive(RightArc, LeftArc);
      }
      else
      {
        drive(0, 0);
      }
    pros::delay(10);
  }
}
pros::Task Drive_pid_task(Drive_pid, nullptr, "drive_pid");
int flip = -1;
void Set_Drive(int typeinput, float Target, int max, float kp)
{
  Kp = kp;
  maxval = max;
  currentdrivetype = typeinput;
  if(currentdrivetype == Drive || currentdrivetype == Line)
  {
    Left_Target_Value = LeftEncoderval()  + (Target*tick_per_inch);
    Right_Target_Value = RightEncoderval() + (Target*tick_per_inch);
  }
  else if (currentdrivetype == Turn || currentdrivetype == RSwing || currentdrivetype == LSwing){
    angle = Target * flip;
  }
  else if (currentdrivetype == Arc)
  {
    angle = Target;
    max = LMax;
    kp = RMax;
  }
}

//___________________________________________________________________________________________________________________________
void set_angle(int Angle)
{
  angle = Angle;
}
void
Turn_pid(void*)
 {

  while (true)
   /*

     Gyro_P = angle - InertialReadings();
     GOutput = Gyro_P * Gyro_Kp;
     //Output = clipit(maxval, minval, Output);
     printf("Output = %d \n", GOutput);
     if (pros::millis()>=2000)
     {
     //  printf("working \n");
       drive(-GOutput, GOutput);
     }
     else
     {
       //printf("millis = %d \n", pros::millis());
       //printf("Almost working \n");
       drive(0, 0);
     }
    //pid goes here
    */
    pros::delay(10);
}
pros::Task Turn_pid_task(Turn_pid, nullptr, "Turn_pid");
int delay_time = 10;
void
wait_drive() {
  int i = 0;
  int k = 0;
  while (true) {
    if (currentdrivetype == Drive) {
      pros::delay(delay_time);
      if (abs(Left_Target_Value-LeftEncoderval())<50 && abs(Right_Target_Value-RightEncoderval())<50) {
        pros::delay(50);
        return;
      }
      if (abs(Left_Target_Value-LeftEncoderval())<150 && abs(Right_Target_Value-RightEncoderval())<150) {
        i+=delay_time;
        if (i>300)
        {
          //pros::delay(50);
          return;
        }
      }
      else
      {
        i = 0;
      }

      if (velocityLeft() == 0 && velocityRight() == 0) {
        k+=delay_time;
        if (k>500) {
          return;
        }
      }
      else
      {
        k = 0;
      }
    }

    else if (currentdrivetype == Turn || currentdrivetype == RSwing || currentdrivetype == LSwing) {
      pros::delay(delay_time);
      if (fabs(angle-InertialReadings())<2) {
        pros::delay(50);
        return;
      }
      if (fabs(angle-InertialReadings())<10) {
        i+=delay_time;
        if (i>400)
        {
          pros::delay(50);
          return;
        }
      }
      else
      {
        i = 0;
      }

      if (left_back.get_actual_velocity()==0 && right_back.get_actual_velocity()==0) {
        k+=delay_time;
        if (k>300) {
          return;
        }
      }
      else
      {
        k = 0;
      }
    }


  }

}
