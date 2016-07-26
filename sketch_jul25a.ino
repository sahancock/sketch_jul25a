#include <NewPing.h>

#define SONAR_NUM      2 // Number of sensors.
#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(4, 5, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(6, 7, MAX_DISTANCE),
  //NewPing(8, 9, MAX_DISTANCE)
};

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
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
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer()) {
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    pingResult(currentSensor);
  }
}

void pingResult(uint8_t sensor) { // Sensor got a ping, do something with the result.
   //The following code would be replaced with your code that does something with the ping result.
// 
    if ((cm[sensor] < 20)){
    Serial.print("MOVE AWAY YOUR TOO CLOSE !!!!!!");
    Serial.println(" ");
    digitalWrite(13,LOW);   
   }
    else if((cm[sensor] < 35)){
    Serial.print("BE CAREFUL I CAN HURT YOU !!!!!!");
    Serial.println(" ");
  }
  else{
      digitalWrite(13,HIGH);
  }

    
  Serial.print(sensor);
  Serial.print(" ");
  Serial.print(cm[sensor]);
  Serial.println("cm"); 
}

 
