#include <IRremote.hpp>
#define IR_RECEIVE_PIN 2

void setup() {
  Serial.begin(115200);                                     // Establish serial communication
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  // Start the receiver
}

void loop() {
  if (IrReceiver.decode()) {
    // Print "old" raw data
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    // Print complete received data in one line
    IrReceiver.printIRResultShort(&Serial);
    // Print the statement required to send this data
    IrReceiver.printIRSendUsage(&Serial);

    IrReceiver.resume();  // Enable receiving of the next value
  }
}
