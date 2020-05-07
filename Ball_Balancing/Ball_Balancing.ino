/**
  ******************************************************************************
  * @Project      Ball Balancing
  * @author       Renardi Adryantoro Priambudi
  * @date         23 - Desember - 2019
  * @Institution  Politeknik Elektronika Negeri Surabaya
  * @Batch        Computer Engineering 2016
  ******************************************************************************
  */



#include <Servo.h>

#define echoPin 2
#define trigPin 4

#define pinServo 11
// defines variables
long duration;
float distance;

Servo servo;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 servo.attach(pinServo);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  float angle;
  float distance;
  distance = readUltrasonic();
  angle = fuzzyControl(distance);
  //angle = 80;
  servo.write(angle); 
  
  Serial.print("Distance = ");
  Serial.print(distance);

  Serial.print(" angle = ");
  Serial.println(angle);
  
  delay(30);
}

float readUltrasonic(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
 // Serial.print("Distance: ");
 // Serial.println(distance);

  return distance;
  }



int fuzzyControl(float distanceToBall){
static float valueDistToBall[4];
static float lowLever,flatLever,highLever;
static float valueAngle[4],valueAngleOutput[4],angleServo; 
static float pembilangCOG,penyebutCOG; 

//Reset variable.
pembilangCOG =0;
penyebutCOG  =0;

//  //=====Fuzzyfication====//
//  //Membership Close
//   if(distanceToBall<4){valueDistToBall[0] = 1;}
//   else if(distanceToBall>=4 && distanceToBall<=14){valueDistToBall[0] = (14 - distanceToBall)/(14-4);}
//   else valueDistToBall[0] = 0;
//   
// //Membership Center
//  if(distanceToBall <10 || distanceToBall>20){valueDistToBall[1]=0; }
//  else if(distanceToBall>=10 && distanceToBall<=15){ valueDistToBall[1] = (distanceToBall-10)/(15 - 10); }
//  else if(distanceToBall>=15 && distanceToBall<=20){ valueDistToBall[1] = (20 - distanceToBall)/(20 - 15);}
//
//  //Membership Far
//  if(distanceToBall>22){valueDistToBall[2]=1; }
//  else if(distanceToBall>=18 && distanceToBall<=22){ valueDistToBall[2] = (distanceToBall - 18)/(22 - 18); }
//  else valueDistToBall[2]=0;
// 

  //=====Fuzzyfication====//
  //Membership Close
   if(distanceToBall<0){valueDistToBall[0] = 1;}
   else if(distanceToBall>=0 && distanceToBall<=14){valueDistToBall[0] = (14 - distanceToBall)/(14-0);}
   else valueDistToBall[0] = 0;
   
 //Membership Center
  if(distanceToBall< 0 || distanceToBall>50){valueDistToBall[1]=0; }
  else if(distanceToBall>=0 && distanceToBall<=15){ valueDistToBall[1] = (distanceToBall-0)/(15 - 0); }
  else if(distanceToBall>=15 && distanceToBall<=50){ valueDistToBall[1] = (50 - distanceToBall)/(50 - 15);}

  //Membership Far
  if(distanceToBall>150){valueDistToBall[2]=1; }
  else if(distanceToBall>=15 && distanceToBall<=150){ valueDistToBall[2] = (distanceToBall - 15)/(150 - 15); }
  else valueDistToBall[2]=0;
 


  //=====RUlE====//
  //close intermediate  far
  //  LL    FL         HL
  lowLever  = valueDistToBall[0];
  flatLever = valueDistToBall[1];
  highLever = valueDistToBall[2];




  //Deffuzification
  for(float angle = 20 ; angle<=180 ; angle++){
  //Membership LOW Lever
    if(angle<35){valueAngle[0]=1; }
    else if(angle>=35 && angle<=70){ valueAngle[0] = (70 - angle)/(70 - 35); }
    else valueAngle[0]=0;
    
  //Membership Flat Lever
    if(angle<60 || angle>80){valueAngle[1]=0; }
    else if(angle>=60 && angle<=70){ valueAngle[1] = (angle-60)/(70 - 60); }
    else if(angle>=70 && angle<=80){ valueAngle[1] = (80 - angle)/(80 - 70); }
    
  //Membership HIGH Lever
    if(angle>140){valueAngle[2]=1; }
    else if(angle>=70 && angle<=140){ valueAngle[2] = (angle - 70)/(140 - 70); }
    else valueAngle[2]=0;

   // Create NEW Membership Fucntion for output
    if(valueAngle[0]>lowLever){valueAngleOutput[0] = lowLever;}
    else if (valueAngle[0]<=lowLever){valueAngleOutput[0] = valueAngle[0];}

    if(valueAngle[1]>flatLever){valueAngleOutput[1] = flatLever;}
    else if (valueAngle[1]<=flatLever){valueAngleOutput[1] = valueAngle[1];}

    if(valueAngle[2]>highLever){valueAngleOutput[2] = highLever;}
    else if (valueAngle[2]<=highLever){valueAngleOutput[2] = valueAngle[2];}

    angleServo = max(valueAngleOutput[0],valueAngleOutput[1]);
    angleServo = max(angleServo,valueAngleOutput[2]);

    pembilangCOG += angleServo*angle;
    penyebutCOG  += angleServo;
  }

  
return (pembilangCOG/penyebutCOG);
}
