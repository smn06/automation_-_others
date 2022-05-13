#include <Servo.h>
#include <SoftwareSerial.h>
String cmd;
String d1;
int flag = 0;

# define hault 0
# define follow 1
# define none 2

const int pow = 100;
const int init = 200;
const int t = 1;
float t_1 = 2;

const int led = 13;
const int bp = 10;

const int lfs0 = 10; 
const int lfs1 = 11; 
const int lfs2 = 12; 
const int lfs3 = 15;
const int lfs4 = 14;

int sensor[5]={0, 0, 0, 0, 0};
float Kp=50;
float Ki=0;
float Kd=0;

float e=0, p=0, i=0, d=0, pidv=0;
float pE=0, pI=0;

#define r 1
#define l -1

Servo leftServo;
Servo rightServo;
void setup() 
{


    pinMode(led, OUTPUT);
    pinMode(bp, INPUT_PULLUP);

    pinMode(lfs0, INPUT);
    pinMode(lfs1, INPUT);
    pinMode(lfs2, INPUT);
    pinMode(lfs3, INPUT);
    pinMode(lfs4, INPUT);

    leftServo.attach(5);
    rightServo.attach(3);

    while (digitalRead(bp))
    {  
    }

    }

void loop() 
    {
    lfsval();    
    switch (flag)
    {
    case hault: 
        stop();
        blink();
        pE = e;
        break;

    case none:  
        stop();
        turn(l, 180);
        pE = 0;
        break;

    case follow:     
        pidal();
        control();    
        break;     
    }
    }

void read()
{
    sensor[0] = digitalRead(lfs0);
    sensor[1] = digitalRead(lfs1);
    sensor[2] = digitalRead(lfs2);
    sensor[3] = digitalRead(lfs3);
    sensor[4] = digitalRead(lfs4);

    if((     sensor[0]== 0 )&&(sensor[1]== 0 )&&(sensor[2]== 0 )&&(sensor[3]== 0 ))  
        {flag = follow; e = 4;}
    else if((sensor[0]== 0 )&&(sensor[1]== 0 )&&(sensor[2]== 0 )&&(sensor[3]== 1 ))  
        {flag = follow; e = 3;}
    else if((sensor[0]== 0 )&&(sensor[1]== 0 )&&(sensor[2]== 0 )&&(sensor[3]== 1 ))  
        {flag = follow; e = 2;}
    else if((sensor[0]== 0 )&&(sensor[1]== 0 )&&(sensor[2]== 1 )&&(sensor[3]== 1 ))  
        {flag = follow; e = 1;}
    else if((sensor[0]== 0 )&&(sensor[1]== 0 )&&(sensor[2]== 1 )&&(sensor[3]== 0 )) 
        {flag = follow; e = 0;}
    else if((sensor[0]== 0 )&&(sensor[1]== 1 )&&(sensor[2]== 1 )&&(sensor[3]== 0 ))  
        {flag = follow; e =- 1;}
    else if((sensor[0]== 0 )&&(sensor[1]== 1 )&&(sensor[2]== 0 )&&(sensor[3]== 0 ))  
        {flag = follow; e = -2;}
    else if((sensor[0]== 1 )&&(sensor[1]== 1 )&&(sensor[2]== 0 )&&(sensor[3]== 0 ))
        {flag = follow; e = -3;}
    else if((sensor[0]== 1 )&&(sensor[1]== 0 )&&(sensor[2]== 0 )&&(sensor[3]== 0 ))  
        {flag = follow; e = -4;}
    else if((sensor[0]== 1 )&&(sensor[1]== 1 )&&(sensor[2]== 1 )&&(sensor[3]== 1 ))  
        {flag = hault; e = 0;}
    else if((sensor[0]== 0 )&&(sensor[1]== 0 )&&(sensor[2]== 0 )&&(sensor[3]== 0 ))  
        {flag = none; e = 0;}
}


void blink(void)
{
for (int i = 0; i<4; i++)
{ 
    digitalWrite (led, HIGH);
    delay (100);
    digitalWrite (led, LOW);
    delay (100);
} 
}



void mcmd()
{
    switch (cmd[0])
    {
    case 'f': 
        stop();
        break;

    case 'w':  
        forward();  
        break;

    case 'd':     
        turn(r, 200);
        stop();
        
        break;

    case 'a': 
        turn(l, 200);
        stop();
        break;

    case 's':  
        backward();
        break;
        
    }
}


void sbtdata (int a) 

{
    digitalWrite (led, HIGH);
    digitalWrite (led, LOW);
}


void pidal()
{
    p = e;
    i = i + e;
    d = e-pE;
    pidv = (Kp*p) + (Ki*i) + (Kd*d);
    pE = e;
}

void screen()
{
    int lfr0 = digitalRead(lfs0);
    int lfr1 = digitalRead(lfs1);
    int lfr2 = digitalRead(lfs2);
    int lfr3 = digitalRead(lfs3);
    int lfr4 = digitalRead(lfs4);
    
    Serial.print (" 0 1 2 3 "); 
    Serial.print (lfr0); 
    Serial.print (" ");
    Serial.print (lfr1); 
    Serial.print (" ");
    Serial.print (lfr2); 
    Serial.print (" ");
    Serial.print (lfr3); 
    Serial.print (" ");
    Serial.print (lfr4); 

    Serial.print ("p: ");
    Serial.print (p);
    Serial.print ("i: ");
    Serial.print (i);
    Serial.print ("d: ");
    Serial.print (d);
    Serial.print ("pid: ");
    Serial.println (pidv);
}

void stop()
{
    leftServo.writeMicroseconds(1000);
    rightServo.writeMicroseconds(1000);
    delay(200);
}

void forward()
{
    leftServo.writeMicroseconds(1000 - pow);
    rightServo.writeMicroseconds(1000 + pow*t);
}

void backward()
{
    leftServo.writeMicroseconds(1000 + pow);
    rightServo.writeMicroseconds(1000 - pow);
}

void ftime (unsigned int time)
{
    forward();
    delay (time);
    stop();
    }

void btime (unsigned int time)
{
  backward();
  delay (time);
  stop();
}

void turn(int direction, int degrees)
{
  leftServo.writeMicroseconds(1000 - init*direction);
  rightServo.writeMicroseconds(1000 - init*direction);
  delay (round(t_1*degrees+1));
  stop();
}

void control()
{

    int leftMotorSpeed = 1000 - init - pidv;
    int rightMotorSpeed = 1000 + init*t - pidv;


    constrain(leftMotorSpeed, 1000, 1500);
    constrain(rightMotorSpeed, 1000, 1500);

    leftServo.writeMicroseconds(leftMotorSpeed);
    rightServo.writeMicroseconds(rightMotorSpeed);


}


void dp(int time, int s) 
{
    for (int i = 0; i<s; i++)
    {
        ftime (time);
        turn(r, 360/s);
    }

}
