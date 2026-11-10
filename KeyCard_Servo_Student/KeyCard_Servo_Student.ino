/*
  ============================================================
   SECRET AGENT VAULT  -  Student Version
  ============================================================
  MISSION: Build a vault that only opens for the right key card.
           Scan the wrong card and it stays locked.

  Name: ______________________      Date: ______________

  ------------------------------------------------------------
  WIRING  (check with your teacher before plugging in the USB!)
  ------------------------------------------------------------
  RC522 reader  (careful: it uses 3.3V, NOT 5V!)
     SDA  -> Pin 10
     SCK  -> Pin 13
     MOSI -> Pin 11
     MISO -> Pin 12
     RST  -> Pin 9
     GND  -> GND
     3.3V -> 3.3V

  Servo
     Signal (orange/yellow) -> Pin 6
     Power  (red)           -> 5V
     Ground (brown/black)   -> GND

  ------------------------------------------------------------
  HOW THIS LESSON WORKS
  ------------------------------------------------------------
  This code has blanks (____). It will NOT upload until you
  fill in every one. Follow the STEP instructions below.

  PART A - Find your card's secret number (UID)
     1. Fill in STEPS 1, 2 and 4 first. For STEP 3 leave the
        UID blank for now by typing "none" between the quotes.
     2. Fill in STEPS 5, 6 and 7 too, then upload.
     3. Open the Serial Monitor (set to 9600) and scan your tag.
     4. The Serial Monitor prints:  UID tag : xxxxxxxx
        Write your UID here: ______________________________

  PART B - Lock it to your card
     5. Go back to STEP 3 and replace "none" with YOUR UID.
        Letters must be lowercase, no spaces.
     6. Upload again.
     7. Scan YOUR card  -> vault opens, waits, closes.
        Scan a FRIEND's card -> ACCESS DENIED.

  IF IT DOESN'T WORK, CHECK:
     - Is your UID typed exactly like the Serial Monitor shows it?
     - Did every line end with a semicolon ; ?
     - Is the RC522 on 3.3V, not 5V?
     - Servo jittering or hitting the cardboard? Change 90 to 70 or 80.

  ------------------------------------------------------------
  STEP HINTS
  ------------------------------------------------------------
  STEP 1: The servo library is called Servo.
          The line should read:  #include <Servo.h>
  STEP 2: To make a servo, write the word Servo then a name.
          The name (vaultServo) is already given.
  STEP 3: Put YOUR card's UID between the quotes.
  STEP 4: Two commands, both start with vaultServo.
            attach(pin)  - connects the servo to a pin
            write(angle) - moves the servo to an angle
          Start the servo closed at angle 0.
  STEP 5: Which variable do we compare the scanned card to?
          It's the one you filled in at STEP 3.
  STEP 6: delay(1000) waits 1 second.
          How many milliseconds is 3 seconds?
          Open = 90, then wait, then closed = 0.
  STEP 7: Type the message the vault shows for a wrong card.

  ------------------------------------------------------------
  COMMAND CHEAT SHEET
  ------------------------------------------------------------
     vaultServo.attach(6)    connects the servo to pin 6
     vaultServo.write(90)    turns the servo to 90 degrees
     delay(1000)             waits 1 second
     Serial.println("Hi")    prints a message
     if (a == b)             checks if two things match
     ||                      means "or"

  ------------------------------------------------------------
  BONUS CHALLENGES (only when your vault works!)
  ------------------------------------------------------------
  1. TWO KEYS: Let a friend's card open the vault too.
     Make a second variable (friendKey) and use:
        if (content == masterKey || content == friendKey) {
  2. PERSONAL GREETING: Make the Serial Monitor say
     "Welcome, [your name]!" when you scan.
  3. BREAK-IN ALARM: Count wrong scans. After 3 wrong cards,
     print "LOCKDOWN!" and ignore every card for 10 seconds.

  ------------------------------------------------------------
  SECTION 5 (away from the computer): DECORATE YOUR VAULT
  ------------------------------------------------------------
  Use the cardboard, paper and glue to build your vault around
  the servo. Make the lid or door open when the servo turns.
  Draw a scanner panel over the RC522 (the card still works
  through thin paper). Give your vault a name!
  ============================================================
*/

#include <SPI.h>
#include <MFRC522.h>

// STEP 1: add the servo library here
#include <____.h>

#define SS_PIN 10     // Pin connected to SDA of the RFID module
#define RST_PIN 9     // Pin connected to RST of the RFID module
#define SERVO_PIN 6   // Pin connected to the servo signal wire

MFRC522 rfid(SS_PIN, RST_PIN);   // Create the RFID reader object

// STEP 2: make a servo object called vaultServo
____ vaultServo;

// STEP 3: store YOUR card's UID between the quotes
String masterKey = "____";

void setup() {
  Serial.begin(9600);   // Start serial communication
  SPI.begin();          // Begin SPI communication
  rfid.PCD_Init();      // Initialize the RFID module

  // STEP 4: connect the servo to its pin, then start it closed (0)
  vaultServo.____(SERVO_PIN);
  vaultServo.____(____);

  Serial.println("Vault locked. Scan your card.");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {   // Is a new card near the reader?
    if (rfid.PICC_ReadCardSerial()) {   // Read the card's UID

      // ---- ALREADY WRITTEN FOR YOU: reads the card ----
      String content = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
        content += String(rfid.uid.uidByte[i], HEX);
      }
      Serial.print("UID tag : ");
      Serial.println(content);
      // -------------------------------------------------

      // STEP 5: if the scanned card matches masterKey...
      if (content == ____) {
        Serial.println("ACCESS GRANTED");

        // STEP 6: open the vault (90), wait 3 seconds, then close it (0)
        vaultServo.write(____);
        delay(____);
        vaultServo.write(____);

      } else {
        // STEP 7: print the message for a wrong card
        Serial.println("____");
      }

      rfid.PICC_HaltA();   // Stop reading the card
    }
  }
}
