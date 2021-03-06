#include <NewPing.h>

#define SONAR_NUM      2 // Number of sensors.
#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

//Our Added Variables
boolean sonarFlag = true;                //Sets control flag for making starting and stopping features

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
	NewPing(4, 5, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
	NewPing(6, 7, MAX_DISTANCE),
	//NewPing(8, 9, MAX_DISTANCE)
};

void setup() {
	pinMode(13, OUTPUT);                    //Turns pin13 on
	Serial.begin(9600);
	pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
	for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
	pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop() {
	for (uint8_t i = 0; i < SONAR_NUM; i++) {// Loop through all the sensors.   
		if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
		  pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
		  sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
		  currentSensor = i; // Sensor being accessed. 
		  cm[currentSensor] = -1; // Make distance zero in case there's no ping echo for this sensor.
      
		  sonar[currentSensor].ping_timer(echoCheck);// Do the ping (processing continues, interrupt will call echoCheck to look for echo).
		}
	}
  //Reads the sonarFlag and turns the LED on if true and off if false
  if(sonarFlag == true){                       
    digitalWrite(13,HIGH);
  }
  else{                               
    digitalWrite(13,LOW);
  }
}

void echoCheck() { // If ping received, set the sensor distance to array.
	if (sonar[currentSensor].check_timer()) {
  		cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
  		pingResult(currentSensor);
  } 
}

void pingResult(uint8_t sensor) { // Sensor got a ping, do something with the result.
    
    //Accepts sensor data from echoCheck and sets the sonarFlag to true or false
    int group = 0;
	  if ((cm[sensor] < 20)){
      sonarFlag = false;
      group = 1;   
  	}
    else if((cm[sensor] < 35)){
      group = 2;  
  	}
  	else{
      sonarFlag = true;
      group = 3;
  	}    

    //Displays the sensor data
	  Serial.print("   GROUP:  ");
    Serial.println(group);
    
	  Serial.print("  SENSOR:  ");
    Serial.println(sensor);
    
  	Serial.print("DISTANCE:  ");
    Serial.print( cm[sensor]); 
    Serial.println("cm");
    
  	Serial.print("   LIGHT:  ");
    Serial.println(sonarFlag);
    
    Serial.println("-------------------");
}
