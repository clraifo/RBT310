const int trigPin = D2;                 // Trigger pin of the ultrasonic sensor
const int echoPin = D3;				    // Echo pin of the ultrasonic sensor
const int greenLED = D4;				// Green LED pin        
const int yellowLED = D5;   	        // Yellow LED pin
const int redLED = D6;				    // Red LED pin
const int buzzer = D7;				    // Buzzer pin

void setup() {
    pinMode(trigPin, OUTPUT);           // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);			// Sets the echoPin as an Input
    pinMode(greenLED, OUTPUT);          // Sets the greenLED as an Output
    pinMode(yellowLED, OUTPUT);		    // Sets the yellowLED as an Output
    pinMode(redLED, OUTPUT);		    // Sets the redLED as an Output
    pinMode(buzzer, OUTPUT);		    // Sets the buzzer as an Output

    // Initiate LEDs and buzzer 
    digitalWrite(greenLED, LOW);        // Sets the greenLED to be off
    digitalWrite(yellowLED, LOW);	    // Sets the yellowLED to be off
    digitalWrite(redLED, LOW);		    // Sets the redLED to be off
    digitalWrite(buzzer, LOW);		    // Sets the buzzer to be off
}

// Function to play a tone on the buzzer
void tone(int pin, unsigned int frequency, unsigned long duration) {    // pin: buzzer pin, frequency: frequency of the tone, duration: duration of the tone
    long half_period = 500000L / frequency;                             // Calculate half period in microseconds
    unsigned long num_cycles = frequency * duration / 1000;             // Calculate number of cycles
    for (unsigned long i = 0; i < num_cycles; i++) {					// Play the tone for the calculated number of cycles
        digitalWrite(pin, HIGH);                                        // Set the buzzer pin to HIGH
        delayMicroseconds(half_period);								    // Wait for half period
        digitalWrite(pin, LOW);										    // Set the buzzer pin to LOW
        delayMicroseconds(half_period);								    // Wait for half period
    }
}

// Main loop
void loop() {                                       // The loop function runs over and over again forever
    long duration, distance;						// Define variables for duration of the ping and the distance result in centimeters

    // Clear the trigPin
    digitalWrite(trigPin, LOW);                     // Clears the trigPin
    delayMicroseconds(2);                           // Delay of 2 microseconds

    // Set the trigPin on HIGH state for 10 microseconds
    digitalWrite(trigPin, HIGH);					// Sets the trigPin on HIGH state
    delayMicroseconds(10);						    // Delay of 10 microseconds
    digitalWrite(trigPin, LOW);						// Sets the trigPin on LOW state

    // Read the echoPin, pulseIn returns the duration in microseconds
    duration = pulseIn(echoPin, HIGH);				// Reads the echoPin, returns the sound wave travel time in microseconds

    // Calculate the distance in cm
    distance = duration * 0.034 / 2;				// Calculating the distance

    // Reset outputs
    digitalWrite(greenLED, LOW);                    // Sets the greenLED to be off
    digitalWrite(yellowLED, LOW);					// Sets the yellowLED to be off
    digitalWrite(redLED, LOW);						// Sets the redLED to be off

    if (distance >= 30 && distance <= 60) {         // If the distance is between 30 and 60 cm
        digitalWrite(yellowLED, HIGH);			    // Turn on the yellow LED
    }
    else if (distance < 30 && distance > 0) {	    // If the distance is less than 30 cm    
        digitalWrite(redLED, HIGH);				    // Turn on the red LED

        // Play a tone on the buzzer
        tone(buzzer, 1000, 950);                    // 1000Hz frequency, 950ms duration to allow a small gap between beeps
    }
    else if (distance > 60 && distance <= 200) {    // If the distance is greater than 60 cm
        digitalWrite(greenLED, HIGH);			    // Turn on the green LED
    }

    // Publish data to Particle Console - WORK IN PROGRESS AS OF 20230809
    String dataString = "Distance: " + String(distance) + " cm, Time: " + String(Time.hour()) + ":" + String(Time.minute()) + ":" + String(Time.second());
    Particle.publish("SensorData", dataString, PRIVATE);

    delay(50);  // Delay for 50 milliseconds before the next loop
}
