/*Proximity detection with audiovisual alerts 

**Particle Photon with HC-SR04 Ultrasonic Sensor**

This code is written  for the Particle Photon microcontroller to interact with the HC-SR04 ultrasonic distance 
sensor and provide feedback using LEDs and a buzzer based on the detected distance.

1. Pin Assignments:
    - The `trigPin` is assigned to D2, which is used to trigger the ultrasonic sound from the HC-SR04.
    - The `echoPin` is assigned to D3, which captures the reflected ultrasonic sound from objects.
    - LEDs are connected to pins D4 (Green), D5 (Yellow), and D6 (Red).
    - The passive buzzer is connected to pin D7.

2. Setup:
    - All pins are initialized. The `trigPin` is set as an OUTPUT since it sends the ultrasonic sound. The `echoPin` 
    is set as an INPUT because it listens for the reflected sound. LEDs and buzzer pins are set as OUTPUTs since 
    they are actuated based on the distance detected.
    - At the start, all LEDs and the buzzer are set to the OFF state.

3. Tone Function:
    - The `tone()` function creates a sound on the passive buzzer by rapidly toggling it on and off at a specified 
    frequency. 

4. Main Loop:
    - The `trigPin` is briefly set HIGH, sending out a burst of ultrasonic sound. This sound travels until it hits 
    an object and then reflects back.
    - The `echoPin` listens for the reflected sound's return. The time taken for the sound to travel out and reflect 
    back is measured.
    - Using this time and the speed of sound, the distance to the object is calculated.
    - Based on the calculated distance:
        - If the object is between 30 and 60 centimeters away, the Yellow LED is turned ON.
        - If the object is less than 30 centimeters away, the Red LED is turned ON, and the buzzer emits a constant tone.
        - If the object is more than 60 centimeters away (but less than 100 centimeters, our rough maximum range for 
        detection), the Green LED is turned ON.
        - Objects beyond 100 centimeters or no object at all won't activate any of the LEDs.

5. Delays:
    - A short delay is added at the end of each loop iteration to prevent excessive readings and to give a clearer 
    indication on the LEDs and buzzer. 

This code offers a simple and effective way to detect distances and give visual and audible feedback based on the distance detected.
*/

// Define pins
const int trigPin = D2;
const int echoPin = D3;
const int greenLED = D4;
const int yellowLED = D5;
const int redLED = D6;
const int buzzer = D7;

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

    // Calculate the distance in cm
    distance = duration * 0.034 / 2;

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
    } else if(distance > 60 && distance <= 200) {
        digitalWrite(greenLED, HIGH);
    }

    // Publish data to Particle Console
    String dataString = "Distance: " + String(distance) + " cm, Time: " + String(Time.hour()) + ":" + String(Time.minute()) + ":" + String(Time.second());
    Particle.publish("SensorData", dataString, PRIVATE);

    delay(50);  // Delay for 50 milliseconds before the next loop
}
