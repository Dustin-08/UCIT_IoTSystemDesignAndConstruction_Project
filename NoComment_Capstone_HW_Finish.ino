#include <SoftwareSerial.h>

#define BTtx       8 
#define BTrx       9 

SoftwareSerial BT(BTtx, BTrx);

char data = 0; 


float min = 0; 
float max = 100.0; 


int rgb_red = 7; 
int rgb_green = 6; 
int rgb_blue = 5; 



int led = 10;


int dust_sensor = A0;
float dust_value = 0;
float dustDensityug = 0; 
int sensor_led = 2; 
int sampling = 280;
int waiting = 40;
float stop_time = 9680;


int IN1 = 13; 
int IN2 = 12; 
int ENA = 11; 

void setup(){

  BT.begin(9600); 
  
  
  Serial.begin(9600); 
  pinMode(sensor_led,OUTPUT); 
  pinMode(4, OUTPUT); 

  pinMode(rgb_red, OUTPUT); 
  pinMode(rgb_green, OUTPUT);
  pinMode(rgb_blue, OUTPUT); 

  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(ENA, OUTPUT); 
}

void loop(){
        
  if(BT.available() > 0) {
    data = BT.read(); 
  }
  
  if(data == '0') { 
    Serial.println("Turn OFF");
    analogWrite(rgb_red, 0);
    analogWrite(rgb_green, 0);
    analogWrite(rgb_blue, 0); 
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); 
    digitalWrite(ENA, 0); 
  }
  else if(data == '1') { 
    Serial.println("Turn ON"); 
    if(dustDensityug <= 35.0){ 
      analogWrite(rgb_red, 0); 
      analogWrite(rgb_green, 0);
      analogWrite(rgb_blue, 255); 
      Serial.print("    "); 
      Serial.println("blue"); 
      digitalWrite(IN1, HIGH); 
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 50);
    }
    else if(35.0 < dustDensityug && dustDensityug <= 75.0){ 
      analogWrite(rgb_red, 0);
      analogWrite(rgb_green, 255); 
      analogWrite(rgb_blue, 0);
      Serial.print("    ");
      Serial.println("green");
      digitalWrite(IN1, HIGH); 
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 180); 
    }
    else if(dustDensityug > 75.0){ 
      analogWrite(rgb_red, 255);
      analogWrite(rgb_green, 0); 
      analogWrite(rgb_blue, 0);
      Serial.print("    ");
      Serial.println("red");
      digitalWrite(IN1, HIGH); 
      digitalWrite(IN2, LOW); 
      analogWrite(ENA, 255);
    }
  }
  else if(data == '2'){
    Serial.println("필터를 소독합니다.");
    digitalWrite(led,HIGH);
    delay(3000);
    digitalWrite(led,LOW);
  }


  digitalWrite(sensor_led, LOW);
  delayMicroseconds(sampling);

  dust_value = analogRead(dust_sensor); 

  delayMicroseconds(waiting);

  digitalWrite(sensor_led, HIGH);
  delayMicroseconds(stop_time);

  dustDensityug = (0.17*(dust_value * (5.0 / 1024))-0.1)*1000;
  if(dustDensityug < min){
    Serial.print("Dust Density [ug/m3]:");
    Serial.println("0");
    BT.print("0");
  }
  else if(dustDensityug > min && dustDensityug <= 35.0){
    Serial.print("Dust Density [ug/m3]:");
    Serial.println(dustDensityug);
    BT.print(dustDensityug);
  }
  else if(dustDensityug > 35.0 && dustDensityug < 75){
     Serial.print("Dust Density [ug/m3]:");
    Serial.println(dustDensityug);
    BT.print(dustDensityug);
  }
  else if(dustDensityug >= max){
    Serial.print("Dust Density [ug/m3]:");
    Serial.println("100");
    BT.print("100");
  }

  delay(3000);
}