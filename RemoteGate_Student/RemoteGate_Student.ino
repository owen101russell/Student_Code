// -----------------------------
// IR Remote → Servo Gate
// -----------------------------

#include <IRremote.hpp>
#include <Servo.h>

const byte IR_RECEIVE_PIN = 2;
const byte SERVO_PIN = 9;

Servo gateServo;


// Replace these with the commands from your remote.
const byte CMD_OPEN  = 0x15;
const byte CMD_CLOSE = 0x7;


void setup() {

  Serial.begin(115200);

  // Attach the servo.
  gateServo.attach(SERVO_PIN);

  // Start with the gate closed.
  gateServo.write(0);

  // Start the IR receiver.
  // Hint: You used this in the previous challenge.
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR servo gate ready");
}


void loop() {

  // Check if a button has been pressed.
  // Hint: You used this in the previous challenge.
  if (IrReceiver.decode()) {

    // Store the command received from the remote.
    // Hint: IrReceiver.decodedIRData.command
    byte command = IrReceiver.decodedIRData.command;


    // If the command is the OPEN command...
    if (command == CMD_OPEN) {

      // Open the gate.
      gateServo.write(90);

      Serial.println("GATE OPEN");


    // Otherwise, if the command is the CLOSE command...
    } else if (command == CMD_CLOSE) {

      // Close the gate.
      gateServo.write(0);

      Serial.println("GATE CLOSED");
    }


    // Get ready to receive another button press.
    // Hint: IrReceiver.resume()
    IrReceiver.resume();
  }
}