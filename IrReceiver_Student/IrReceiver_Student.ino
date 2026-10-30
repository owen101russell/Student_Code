// -----------------------------
// IR Remote → LED
// -----------------------------

#include <IRremote.hpp>

// IR receiver is connected to pin 2.
const byte IR_RECEIVE_PIN = 2;

// LED is connected to pin 13.
const byte LED_PIN = 13;


void setup() {

  // Start Serial communication.
  Serial.begin(115200);

  // Set the LED pin as an output.
  pinMode(LED_PIN, OUTPUT);

  // Start the IR receiver.
  // Hint: IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK)
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR receiver ready");
}


void loop() {

  // Check if the IR receiver has received a signal.
  // Hint: The IRremote library uses IrReceiver.decode()
  if (IrReceiver.decode()) {

    // Print the IR information to the Serial Monitor.
    // Hint: The library has printIRResultShort(&Serial)
    IrReceiver.printIRResultShort(&Serial);

    // Print the command received from the remote.
    // Hint: IrReceiver.decodedIRData.command
    Serial.print("Command: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    // Turn the LED on.
    digitalWrite(LED_PIN, HIGH);

    // Wait 100 milliseconds.
    delay(100);

    // Turn the LED off.
    digitalWrite(LED_PIN, LOW);

    // Tell the IR receiver to get ready for another signal.
    // Hint: The library uses IrReceiver.resume()
    IrReceiver.resume();
  }
}
