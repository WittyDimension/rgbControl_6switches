/*
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------
 * PROBLEM STATEMENT →
 * Imagine You have the following parts with you - 
1. An Arduino Uno 3.3V, 
2. You have 6 tactile switches,
3. One RGB LED (Analog),
4. One battery management chip.  The Battery management chip provides you with an analog value indicating 
the percentage of the battery (0v for 0%~1.8V for 100%). 

Design schematic and an Arduino code so that the circuit can do the following function. Assume you have 
access to any passive component, LDO. If any information is missing (e.g., pinout of the battery management chip) 
make educated assumptions.
 
1. Take power from a battery and read the % of the battery also.
2. One switch puts the entire system to sleep, turns off the LED.
3. One Switch wakes up the system from Sleep.
4. One Switch to select the LED Color (Red, Blue, Green).
5. Two switch one to reduce and increase the intensity.
6. One switch to show the % percentage of battery in any creative way possible you can think of through the LED

The Schematic can be on Paper or on E-CAD software.

RULES TO FOLLOW
You will be graded on 
1. Design of Circuit considering good practices
2. Code clarity, commenting, formatting.

Hint: An example of good practices -> A circuit which avoids floating pins, debounce methods 

------------------------------------------------------------------------------------------------------------------------------------------------------------
Generally, Arduino requires 7-12V to operate
For our case, 1.8V * 4 = 7.2V, as the BMS chip was mentioned as 1.8V.

Here the Analog Pin A0 is used to read the battery percentage value.
------------------------------------------------------------------------------------------------------------------------------------------------------------
Code by : WittyDimension
Date of Creation: 24-10-2021
------------------------------------------------------------------------------------------------------------------------------------------------------------
 */

#include <avr/sleep.h>      // Library that contains the sleep modes
#define swOn 2              // Interrupt Pin; Pin to use to wake up the Arduino


// Constants that won't change. Setting pin numbers for the switches
const int swOff = 3;                       // Pin to make the Arduino to goto sleep mode
const int greenPin = 4;                    // Pin to control the green terminal of the RGB LED
const int bluePin  = 5;                    // Pin to control the blue terminal of the RGB LED
const int redPin   = 6;                    // Pin to control the red terminal of the RGB LED
const int csPin    = 8;                    // Pin to select the color of the RGB LED; Color Select Pin 
const int plusIn   = 9;                    // Pin to increase the intensity of the RGB LED
const int minusIn  = 10;                   // Pin to decrease the intensity of the RGB LED
const int battPercentPin  = 11;            // Pin (to press)to determine the battery percentage

// Constants to set minimum and maximum intensity of the LED brightness
const int minBrightness  = 0;
const int maxBrightness  = 225;
int brightness = 125;               // A random values is chosen for initialization
byte fadeIncrement = 5;             // The value of the amount of increment/decrement of the intensity values
int debounceDelay   = 50;           // A kind of delay between two events.


// Variables that will change value
int ledPin;                        // The value of the pin number of the LED in action
int counter = 0;                   // To count the number of clicks; set to avoid floating values
float battPercent = 0.0;           // Initializing the battery percentage; set to avoid floating values


void setup() {
  // Setup code here, to run once when reset is pressed:
  
  pinMode(swOn, INPUT);
  pinMode(swOff, INPUT);
  pinMode(csPin, INPUT);
  pinMode(plusIn, INPUT);
  pinMode(minusIn, INPUT);
  pinMode(battPercentPin, INPUT);
  
  // Setting the pins of LEDs
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initializing serial communication
  Serial.begin(115200);

  // Setting the built-in led
  digitalWrite(LED_BUILTIN, HIGH);
}


void loop() {
  // Main code here, to run repeatedly:

  // setting the time for the events
  unsigned long currentMillis = millis();
  
  if (digitalRead(swOn) == LOW) {
    // The switch swOn is pressed to wake the system from sleep

    digitalWrite(LED_BUILTIN, HIGH);      // turning the led on
    digitalWrite(redPin, HIGH);           // turning the RGB LED on with Red light
  }

  
  else if (digitalRead(swOff) == HIGH) {
    // The switch swOff is pressed to make the sytem go to sleep mode
    goToSleep();
    rgbLow();
  }

  
  else if (digitalRead(csPin) == LOW) {
    // The switch csPin selects the colour of the RGB LED - Red, Blue or Green
    colorSelect(currentMillis);
  }

  
  else if (digitalRead(plusIn) == HIGH || digitalRead(minusIn) == HIGH) {
    // The switches to control the fade value or the intensity of the LED
    controlIntensity(currentMillis);
  }

  
  else if (digitalRead(battPercentPin) == HIGH) {
    // The switch to show the battery percentage
    // First reset the RGB LED
    rgbLow();
    batteryVoltage();
  }

}


/*
 * The section below contains all other user-defined functions and routines required
 * to run the main program written under the void loop() function.
 */
 
void goToSleep() {
  // Disabling the ADC (Analog to digital converter)
  static byte prevADCSRA = ADCSRA;
  ADCSRA = 0;

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // setting the sleep mode; full sleep
  sleep_enable();
  // First disabling interupts (temporarily) so the wakeUp ISR does not run 
  // before we are asleep and then prevent interrupts and then defining 
  // the ISR (Interrupt Service Routine) to run when triggered awake
  noInterrupts();
  // attaching an interrupt to pin d2; syntax attachInterrupt (interrupt, ISR, mode); 
  // interrupt attached to 2, so digitalPinToInterrupt(2) = 0 and for pin 3 its 1.
  attachInterrupt(digitalPinToInterrupt(swOn), wakeUp, LOW);
  
  digitalWrite(LED_BUILTIN, LOW);       // turning the LED off
  delay(1000);                          // allowing the led be turned off before going to sleep

  interrupts();                         // Allowing interrupts now
  sleep_cpu();                          // activating the sleep mode

  Serial.println("Hello, World! Good Morning!");      // debug purpose; code executed after the interrupt
  ADCSRA = prevADCSRA;                                // After it wakes up, re-enabling the ADC as before
}


// ISR(Interrupt Service Routine) for the attachInterrupt.
void wakeUp() {
  Serial.println("Interrupt is Fired");                      // for debugging; print in serial monitor on wakeup
  sleep_disable();                                        // disable sleep mode
  detachInterrupt(digitalPinToInterrupt(swOn));   // removes the interrupt from pin 2
}


// User-defined function to set all the pins of the RGB LED low, to set it to OFF state.
void rgbLow() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}


// User-defined funtion to control the intesity of LED when specified buttons are pressed.
void controlIntensity(unsigned long thisMillis) {
  // This function is used to control the intensity of the the LED light.
  
  if(thisMillis - previousFadeMillis >= debounceDelay) {
    if (digitalRead(plusIn) == HIGH && brightness < maxBrightness){
      brightness = brightness + fadeIncrement; 
      // if the brightness Up button is pushed, add one degree of brightness to the current level
      Serial.println(brightness);     // for debugging purposes
    }
    else if (digitalRead(minusIn) == HIGH && brightness > minBrightness) {
      brightness = brightness - interval;
      // if the brightness Down button is pushed, subtract one degree of brightness from the current level
      Serial.println(brightness);     // for debugging purposes
    }
    analogWrite(ledPin, brightness);  // To update LED when the change happens

    previousFadeMillis = thisMillis;  // To reset millis for the next set of iterations for fade timer
  }
}


// User-defined function to select the color of RGB led according to the number of times the
// key is pressed. The variable 'counter' keeps tab of number of times the button is clicked.
void colorSelect(unsigned long thisMillis) {
  // This function is used to select the colour of the RGB LED - Red, Blue or Green

  if(thisMillis - previousFadeMillis >= debounceDelay) {
    counter++;
    if (counter == 0)
    {
      rgbLow();
    }
    
    else if (counter == 1)
    {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      ledPin = 6;
    }
  
    else if (counter == 2)
    {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      ledPin = 4;
    }
  
    else if (counter == 3)
    {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
      ledPin = 5;
    }
  
    else
    {
      counter = 0;
    }
}



// User-defined function to check the battery percentage.
void batteryVoltage() {
  // This function is used to calculate the battery percentage
  
  int sensorValue = analogRead(A0);   // reading the raw value through the inbuilt ADC

  // Converting the analog reading(0 - 1023) to a voltage:
  float battVolt = (sensorValue * (5.0 / 1023.0)) * 11.0;   // voltage multiplied by 11 as using the voltage divider that divides by 11; provides over voltage protection

  // Arduino requires 7-12V to operate. A 7.2V battery is used to power the Arduino Uno here. 1.8V * 4 = 7.2V
  if (battVolt > 7.2) {
    battPercent = 100;
    analogWrite(greenPin, maxBrightness);                  // Visualization to show that the battery is at full
  }
  else {
    battPercent = ((battVolt / 7.2) * 100);                // Formula to calculate the battery percentage

    // This is a representation of battery voltage percentage. Intensity is mapped to the percent value.
    // 0%  - 20%  - It will be represented in Red color (by the LED).
    // 20% - 100% - It will be represented in Green color (by the LED).
    if (battPercent >=0 && battPercent < 20){
      analogWrite(redPin, map(battPercent, 0, 20, 0, 255));
    }
    else {
      analogWrite(greenPin, map(battPercent, 20, 100, 50, 255)); 
    } 
  }
}
