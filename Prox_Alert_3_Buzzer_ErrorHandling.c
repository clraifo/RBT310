/*
20230815 - Added the following error handling to see if I could pull it off:

**Handling Timeout from pulseIn() Function

  **Purpose: The pulseIn() function waits for the pin to go HIGH, and then starts timing until the pin goes LOW. 
  If the pin remains at its current state for longer than a timeout period (default timeout is 1 second), 
  the function will return 0. This can occur if, for some reason, the sensor didn't detect a reflection (i.e., 
  the signal "timed out" without receiving an echo).

  **Implementation: After calling pulseIn(), I check if duration is 0. If it is, this means there was a timeout 
  and we didn't get a valid reading from the sensor. In this case, I set distance to the MAX_DISTANCE constant 
  (200cm in this case) to signify that the object is out of range.

  duration = pulseIn(echoPin, HIGH);
  if (duration == 0) {
     distance = MAX_DISTANCE;
  } else {
     distance = duration * 0.034 / 2;
  }

**Range Check on the Calculated Distance

  **Purpose: This is a sanity check to ensure that the calculated distance is within expected bounds. If there's 
  any interference, noise, or unexpected behavior from the sensor, it might produce readings that are outside the 
  expected range (either negative or extremely large).

  **Implementation: After calculating the distance, I check if the distance is either less than 0 or greater than 
  MAX_DISTANCE. If it falls outside this range, I reset the distance to MAX_DISTANCE to indicate an out-of-range 
  or erroneous value. This helps ensure that the subsequent logic (like LED actuation) behaves consistently and 
  isn't thrown off by unexpected readings.

  if (distance < 0 || distance > MAX_DISTANCE) {
     distance = MAX_DISTANCE;
  }

*/


// Define pins
const int trigPin = D2;
const int echoPin = D3;
const int greenLED = D4;
const int yellowLED = D5;
const int redLED = D6;
const int buzzer = D7;

const int MAX_DISTANCE = 200; // Maximum expected distance in cm

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(buzzer, OUTPUT);

    // Initiate LEDs and buzzer to be off
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
}

void tone(int pin, unsigned int frequency, unsigned long duration) {
    long half_period = 500000L / frequency;
    unsigned long num_cycles = frequency * duration / 1000;  // Total number of cycles
    for (unsigned long i = 0; i < num_cycles; i++) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(half_period);
        digitalWrite(pin, LOW);
        delayMicroseconds(half_period);
    }
}

void loop() {
    long duration, distance;

    // Clear the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Set the trigPin on HIGH state for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echoPin, pulseIn returns the duration in microseconds
    duration = pulseIn(echoPin, HIGH);

    if (duration == 0) {
       // Handle the error, e.g., set distance to a max value or indicate an error state
       distance = MAX_DISTANCE;
    } else {
       distance = duration * 0.034 / 2;
    }

    // Range Check
    if (distance < 0 || distance > MAX_DISTANCE) {
       // Handle unexpected readings, maybe by ignoring them or logging an error
       // Here, I'll just reset to MAX_DISTANCE to indicate an out-of-range value
       distance = MAX_DISTANCE;
    }

    // Reset outputs
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);

    if(distance >= 30 && distance <= 60) {
        digitalWrite(yellowLED, HIGH);
    } else if(distance < 30 && distance > 0) {
        digitalWrite(redLED, HIGH);
        
        // Play a tone on the buzzer
        tone(buzzer, 1000, 950); // 1000Hz frequency, 950ms duration to allow a small gap between beeps
    } else if(distance > 60 && distance <= MAX_DISTANCE) {
        digitalWrite(greenLED, HIGH);
    }

    // Publish data to Particle Console
    String dataString = "Distance: " + String(distance) + " cm, Time: " + String(Time.hour()) + ":" + String(Time.minute()) + ":" + String(Time.second());
    Particle.publish("SensorData", dataString, PRIVATE);

    delay(50);  // Delay for 50 milliseconds before the next loop
}
