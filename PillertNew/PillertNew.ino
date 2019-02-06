#include <DS1302.h>
//#include <MyRealTimeClock.h>
#include <AFMotor.h>

AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR12_64KHZ);
AF_DCMotor motor4(4, MOTOR12_64KHZ);

 // String inString="";
 DS1302 rtc(42,44,46);
//MyRealTimeClock myRTC(34,36,38); //****

int timeHour[2];   //used to store the drink hour of both the tablet kinds
int timeMin[2];    //used to store the drink minutes of both the tablet kinds
int noTab [2];   //used to store number of tablets to be consumed
Time t;         
bool check=true;// to store and use 
char pillNum; //Used to decide on actions 
int real; // it uses to store the r value

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(22,INPUT);   // ir sensor input pin for box 1 //*1
  pinMode(26,INPUT);   // ir sensor input pin for box 2 //*4
  pinMode(24,OUTPUT);  // Buzzer ring output pin //*2
  digitalWrite(24,LOW); //*2
  digitalWrite(40,HIGH);
  
// myRTC.setDS1302Time(12, 20, 23, 11 , 28, 06 , 2018); //*3
  
  // Set the clock to run-mode, and disable the write protection
 rtc.halt(false);
 rtc.writeProtect(false);

//  rtc.setDOW(THURSDAY);        // Set Day-of-Week to FRIDAY
  //rtc.setTime(23 ,21 , 00 );     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(28, 6 , 2018);   // Set the date to August 6th, 2010
}
  
  

void loop() {
  
  lastTime();
  
  if(check == true){
     real=ringTime();
    Serial.println(real);
  if(real==1)
  {
  //box 1 time reached 
  digitalWrite(24,HIGH);
   check=false;
  }
  else if(real==2)
  {
  //box 2 time reached 
  digitalWrite(24,HIGH);
    check=false;
  }
  else if(real==3)
  {
   //Both Boxes time reached together 
   digitalWrite(24,HIGH); 
     check=false;
     }
  
}
Serial.println(real);
if(Serial1.available() > 0)
  {
    pillNum=Serial1.read();
    if(pillNum=='a' || pillNum=='b')
    {
    setPillert(); //Used for input tablet time 
    }
    else if(pillNum=='c' && check==false)
    {
     // used for eject
     digitalWrite(24,LOW);
    // check=false; //Made False in the eject Method
     eject();
     check=!check;
     Serial.println("Exiting Eject Method");
     
    }
    }

    Time clk=rtc.getTime();
    if(clk.sec==30){
      digitalWrite(24,LOW);
      }
  }

  


int ringTime(){
  int r=0;
  t=rtc.getTime();
 
if(t.hour==timeHour[0] && t.min==timeMin[0] && t.sec==0){
   r=1;
   Serial.println("Time has reached for box 1");
   if(t.hour==timeHour[1] && t.min==timeMin[1] && t.sec==0){
     r=r+2;
     Serial.println("Time has reached for box 2 too");
    }
   }

else if(t.hour==timeHour[1] && t.min==timeMin[1] && t.sec==0){
   r=2;
   Serial.println("Time has reached for box 2");
  }
  Serial.print("R is ");
  Serial.println(r);
  return r;
  }


void setPillert(){ 
  switch(pillNum){
    case 'a':
        Serial.println("Box 1 Selected");
        //Code area for pill box 1

        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        motor1.setSpeed(255);
        motor2.setSpeed(255);
        delay(2000);
        motor1.setSpeed(0);
        motor2.setSpeed(0);
        Serial.println("Insert the pills");
        
        Serial.println("Input Hours");
        Serial1.println("Input Hours");
        while(Serial1.available()==0){}    //Waiting for hour input
        int x;
        x=Serial1.read();
        timeHour[0]=x;
        Serial.print("Hours set to");
        Serial.println(x);
        Serial.println( timeHour[0]);
        Serial1.println("OK");
        
        Serial.println("Input Minutes");
        Serial1.println("Input Minutes");

        while(Serial1.available()==0){}    //Waiting for Minute input 
        int y;
        y=Serial1.read();
        timeMin[0]=y;
        Serial.print("Minutes set to");
        Serial.println(y);
        Serial.println( timeMin[0]);
        Serial1.println("OK");

        
        Serial.println("Input Amount of Tablets");
        Serial1.println("Input Amount of Tablets");
        while(Serial1.available()==0){}   //Waiting for Amount of Tablet input
        int z;
        z=Serial1.read(); 
        noTab[0]=z;
        Serial.print("No of Tablets set to");
        Serial.println(z);
        Serial.println(noTab[0]);
        Serial1.println("OK");
        break;
         
    case 'b':
         Serial.println("Box 2 Selected");

         motor3.run(BACKWARD);
         motor4.run(BACKWARD);
         motor3.setSpeed(255);
         motor4.setSpeed(255);
         delay(3000);
         motor3.setSpeed(0);
         motor4.setSpeed(0);
         Serial.println("Insert the pills");

         
         Serial.println("Input Hours");
         Serial1.println("Input Hours");
         while(Serial1.available()==0){}    //Waiting for hour input
         int a;
         a=Serial1.read();
         timeHour[1]=a;
         Serial.print("Hours set to");
         Serial.println(a);
         Serial.println(timeHour[1]);
         Serial1.println("OK");


         Serial.println("Input Minutes");
         Serial1.println("Input Minutes");
         while(Serial1.available()==0){}    //Waiting for Minute input 
         int b;
         b=Serial1.read();
         timeMin[1]=b;
         Serial.print("Minutes set to");
         Serial.println(b);
         Serial.println( timeMin[1]);
         Serial1.println("OK");


         Serial.println("Input Amount of Tablets");
         Serial1.println("Input Amount of Tablets");
         while(Serial1.available()==0){}   //Waiting for Amount of Tablet input
         int c;
         c=Serial1.read(); 
         noTab[1]=c;
         Serial.print("No of Tablets set to");
         Serial.println(c);
         Serial.println(noTab[1]);
         Serial1.println("OK");
         break; 
     }
  
   Serial.println("Action Completed");
  }

 void boxMotor1(){
  int count=0;   //used to determine the tablet fall
  while(count<noTab [0]){
  while(digitalRead(22)==HIGH){
    
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  Serial.println("Motor 1 and 2 Starts");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
 }
 
  Serial.println("Exited While loop");
 if(digitalRead(22)==LOW){
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    delay(1000);
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    //digitalWrite(24,LOW);
    //delay(2500);
  }
  count++;
  }
  Serial.println("Exits Box 1 Method");
 }

 void boxMotor2(){
  int ting=0;   //used to determine the tablet fall
  while(ting<noTab[1]){
  while(digitalRead(26)==HIGH){
    
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  Serial.println("Motor 3 and 4 Starts");
  motor3.run(FORWARD);
  motor4.run(FORWARD);
 }
 
  Serial.println("IR sensed");
 if(digitalRead(26)==LOW){
    motor3.run (BACKWARD);
    motor4.run(BACKWARD);
    delay(1000);
    motor3.setSpeed(0);
    motor4.setSpeed(0);
   // digitalWrite(24,LOW);
   // delay(2500);
  }
  ting++;
  }
 }

void boxMotor12(){
  int allIn=0;
  int allOut=0;
 
  while(allIn<noTab[0] || allOut<noTab[1])
  {
    if(allIn<noTab[0])
    {
      motor1.run(FORWARD);
      motor2.run(FORWARD);
 
      motor1.setSpeed(255);
      motor2.setSpeed(255);
    }
    if(allOut<noTab[1])
    {
       motor3.run(FORWARD);
       motor4.run(FORWARD);

      motor3.setSpeed(255);
      motor4.setSpeed(255);
    }

    if(digitalRead(22)==LOW){
      //motor1.run(BACKWARD);
      //motor2.run(BACKWARD);
       motor1.setSpeed(0);
       motor2.setSpeed(0);
      allIn++;
      Serial.println("Compartment 1 detected");
      }

    if(digitalRead(26)==LOW){
      //motor3.run(BACKWARD);
      //motor4.run(BACKWARD);
         motor3.setSpeed(0);
         motor4.setSpeed(0);
  
      allOut++;
      Serial.println("Compartment 2 detected");
      }
      
  } 

    motor3.setSpeed(255);
    motor4.setSpeed(255);
    motor1.setSpeed(255);
    motor2.setSpeed(255);
 
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
    delay(4000);
    motor3.setSpeed(0);
    motor4.setSpeed(0);
    motor1.setSpeed(0);
    motor2.setSpeed(0);
 
    
  }

 

void lastTime(){
 t=rtc.getTime();
 Serial.print("Current Date / Time: "); 
 Serial.print(t.date); // Element 1
 Serial.print("/"); 
 Serial.print(rtc.getMonthStr()); // Element 2
 Serial.print("/");
 Serial.print(t.year); // Element 3
 Serial.print(" ");
 Serial.print(t.hour); // El ement 4
 Serial.print(":");
 Serial.print(t.min); // Element 5
 Serial.print(":");
 Serial.println(t.sec); // Element 6
 delay(1000);
}

void eject(){
  
  if(real==1)
  {
    boxMotor1();
  }
  
  else if(real==2)
  {
     boxMotor2();
  }   
   
  else if(real==3)
  {
     boxMotor12(); 
     //boxMotor2();
  }
  
     check=false;
     Serial.print("The Check Value is ");
     Serial.println(check);
     Serial1.println(check);
  }

  
