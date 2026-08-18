#include <IRremote.hpp>
#include <Servo.h>
Servo servo;
// ==========================
// Pin Definitions
// ==========================
const byte IR_RECEIVE_PIN = 2;

// Add your other pin definitions here: 
//Examples: 
// const byte LED_PIN = 13;
// const byte MOTOR_PIN = 5;
// const byte BUTTON_PIN = 7;

// Replace these with the commands from YOUR remote
const byte CMD_BUTTON1 = 0x15; // - Button
const byte CMD_BUTTON2 = 0x07; // + Button
const byte CMD_BUTTONSUN = 0x0;

const byte CMD_BUTTONSUN = 0xC;//1
const byte CMD_BUTTONSUN = 0x18;//2
const byte CMD_BUTTONSUN = 0x5E;//3
const byte CMD_BUTTONSUN = 0x0;
const byte CMD_BUTTONSUN = 0x0;
const byte CMD_BUTTONSUN = 0x0;
const byte CMD_BUTTONSUN = 0x0;
const byte CMD_BUTTONSUN = 0x0;
const byte CMD_BUTTONSUN = 0x0;
const byte CMD_BUTTONSUN = 0x16;//0


// const byte CMD_BUTTON3 = 0x09;
// const byte CMD_BUTTON4 = 0x43;

void setup() {
  Serial.begin(115200);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Remote Project Ready");

servo.attach (8);
}

void loop() {
  if (IrReceiver.decode()) {

    byte command = IrReceiver.decodedIRData.command;

    Serial.print("Command: 0x");
    Serial.println(command, HEX);

    if (command == CMD_BUTTON1) {

      // Do something here

      servo.write(10);

    }
    else if (command == CMD_BUTTON2) {

      // Do something else here

      servo.write(-10);

    }

    if (command == CMD_BUTTONSUN) {
      Serial.println("THE SUN");
      servo.write(50);
      delay(1000);
      servo.write(-50);
    }


    // else if (command == CMD_BUTTON3) {
    //
    //   Your code here
    //
    // }

    // else if (command == CMD_BUTTON4) {
    //
    //   Your code here
    //
    // }
    IrReceiver.resume();
  }
}


