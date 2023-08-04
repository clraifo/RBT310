//Library inclusions for the ultrasonic sensor and the LED display
#include "TM1637Display.h"
#include "Ultrasonic.h"

// Define connections to the Particle Photon
#define TRIGGER_PIN D2
#define ECHO_PIN D3
#define GREEN_LED D4
#define YELLOW_LED D5
#define RED_LED D6
#define CLK_PIN D7
#define DIO_PIN A0 

 
// Create an instance of Ultrasonic and TM1637Display
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
TM1637Display display(CLK_PIN, DIO_PIN);
 
void setup() {
    // Initialize LEDs as output
    pinMode(GREEN_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    
    // Initialize TM1637 display
    display.setBrightness(0x0f); // set the diplay to maximum brightness
    display.clear(); // clear the display
}
 
void loop() {
    // Get the distance from the ultrasonic sensor
    long distance_cm = ultrasonic.read();
 
    // Convert the distance to inches
    float distance_inch = distance_cm * 0.393701;
 
    // Display distance on the 4-digit display
    display.showNumberDec(distance_inch * 10, true, 3, 0);
 
    // Determine which LED to light up based on distance
    if (distance_inch > 24) {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(RED_LED, LOW);
    } else if (distance_inch <= 24 && distance_inch > 12) {
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, LOW);
    } else if (distance_inch <= 12) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
    }
 
    delay(100);
}
