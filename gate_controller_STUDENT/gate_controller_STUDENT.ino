/*
=========================================================
                  GATE CONTROLLER
=========================================================

GOAL

Press a button to open and close a gate. A green light
shows when it's open, a red light shows when it's closed.

---------------------------------------------------------
CONTROLS

Button = OPEN / CLOSE
(press once to open, press again to close)

---------------------------------------------------------
WHAT YOU WILL LEARN

- Using a boolean to track ON / OFF state
- Moving a servo to a fixed position
- Turning LEDs on/off based on state

Reading the button as a single press has already been
done for you.

=========================================================
*/

#include <Servo.h>

Servo gateServo;

const int buttonPin = 2;
const int greenPin = 13;
const int redPin = 12;
const int servoPin = 9;

const int closedAngle = 0;
const int openAngle = 90;

bool gateOpen = false;
bool lastButtonState = HIGH;   // HIGH = not pressed (INPUT_PULLUP)


// ======================================================
// BUTTON HELPER
// ======================================================
//
// You do NOT need to change this section.
//
// Returns true for ONE frame only, on the moment the
// button is first pressed down.
//
// ======================================================

bool buttonPressedEdge()
{
  bool current = digitalRead(buttonPin);
  bool pressed = (lastButtonState == HIGH && current == LOW);
  lastButtonState = current;
  return pressed;
}


// ======================================================
// STEP 1 - TOGGLE THE GATE
//
// This runs every time the button is pressed. Decide
// what should happen:
//
// - Flip gateOpen (true becomes false, false becomes true)
//     gateOpen = !gateOpen;
//
// - Move the servo to match:
//     if gateOpen  -> gateServo.write(openAngle);
//     if !gateOpen -> gateServo.write(closedAngle);
//
// - Set the LEDs to match:
//     if gateOpen  -> green ON,  red OFF
//     if !gateOpen -> green OFF, red ON
//
// ======================================================

void toggleGate()
{
  // Write your gate logic here.

}


// ======================================================
// SETUP
// ======================================================

void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  gateServo.attach(servoPin);
  gateServo.write(closedAngle);

  digitalWrite(redPin, HIGH);   // start closed
  digitalWrite(greenPin, LOW);
}


// ======================================================
// MAIN LOOP
// ======================================================

void loop()
{
  if (buttonPressedEdge())
  {
    toggleGate();
  }
}
