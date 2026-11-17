/*
  ============================================================
   OLED RADAR  -  Student Version
  ============================================================
  MISSION: Build a real radar! A servo will sweep a sound sensor
  left and right. Your code measures the distance and draws
  the radar on the OLED: a sweep line, plus a dot for every
  object it finds.

  Name: ______________________      Date: ______________

  ------------------------------------------------------------
  HOW THE SOUND SENSOR WORKS
  ------------------------------------------------------------
  The sensor is like a bat. It sends out a sound you can't hear
  (TRIG), then listens for it to bounce back (ECHO).
  The longer the echo takes, the further away the object is.

  ------------------------------------------------------------
  WIRING  (check with your teacher before plugging in the USB!)
  ------------------------------------------------------------
  OLED screen      VCC  -> 5V
                   GND  -> GND
                   SDA  -> A4
                   SCL  -> A5

  Sound sensor     VCC  -> 5V
                   GND  -> GND
                   TRIG -> Pin 7
                   ECHO -> Pin 8

  Servo            Signal (orange/yellow) -> Pin 6
                   Power  (red)           -> 5V
                   Ground (brown/black)   -> GND

  Build tip: attach the sensor to the servo arm with cardboard.
  Leave some slack in the sensor wires so they don't pull.

  ------------------------------------------------------------
  HOW TO DO THIS LESSON
  ------------------------------------------------------------
  There are 20 STEPS. Search for the word STEP to find each one.
  Every step has a HINT right above it. Read it first!

  There are two kinds of steps:

     BLANK STEPS   Fill in every ____ .
     HINT STEPS    No code is given. Read the hint and
                   WRITE THE LINE(S) YOURSELF.

  The code will not upload until every step is done.

  ------------------------------------------------------------
  TESTING
  ------------------------------------------------------------
  1. Upload and open the Serial Monitor (9600). You should see
     angles and distances scrolling past.
  2. Put your hand in front of the sensor. The distance should
     drop. If it stays at 999, check your wiring and STEPS 3, 4, 7-11.
  3. Watch the OLED. The sweep line should move and dots should
     appear where your hand (or a cardboard wall) is.

  IF IT DOESN'T WORK, CHECK:
     - Is TRIG on pin 7 and ECHO on pin 8?
     - Is the OLED on SDA = A4 and SCL = A5?
     - Did every line end with a semicolon ; ?
     - Radar looks backwards? Tell your teacher!

  ------------------------------------------------------------
  BONUS CHALLENGES (only when your radar works!)
  ------------------------------------------------------------
  1. Change MAX_DISTANCE to 30 or 100. What changes?
  2. Add a buzzer or LED that turns on when something is
     closer than 10 cm.
  3. Make the blips bigger when the object is closer.
  4. Change the radar sweep speed by changing the delay(60).
  ============================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ------------------------------------------------------------
// STEP 1
// HINT: A library is a pack of ready-made code. The servo
// library lets us control a servo with simple commands.
// It is called Servo, and the line looks like: #include <Name.h>
// ------------------------------------------------------------
#include <____.h>

// ----- OLED settings (already written) -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// ------------------------------------------------------------
// STEP 2
// HINT: A #define gives a pin a name so the code is easier to read.
// The servo signal wire is on pin 6.
// ------------------------------------------------------------
#define SERVO_PIN ____

// ------------------------------------------------------------
// STEP 3   (HINT STEP - write the line yourself)
// HINT: Same idea as STEP 2.
//   Format:   #define NAME PIN_NUMBER
//   Example:  #define LED_PIN 13
// The sensor's TRIG wire is on pin 7. Call it TRIG_PIN.
// ------------------------------------------------------------


// ------------------------------------------------------------
// STEP 4   (HINT STEP - write the line yourself)
// HINT: Same again. The sensor's ECHO wire is on pin 8.
// Call it ECHO_PIN.
// ------------------------------------------------------------


// ----- Radar settings (already written) -----
#define CENTER_X 64        // radar centre (bottom middle of the screen)
#define CENTER_Y 63
#define RADAR_RADIUS 60    // radar size in pixels
#define MAX_DISTANCE 50    // furthest distance shown (cm)

#define MIN_ANGLE 15       // sweep from 15 degrees...
#define MAX_ANGLE 165      // ...to 165 degrees
#define ANGLE_STEP 5       // measure every 5 degrees
#define NUM_POINTS 31      // how many measurements in one sweep

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ------------------------------------------------------------
// STEP 5
// HINT: A servo is a motor that turns to an exact angle.
// To use one, we make a servo object and give it a name.
//   Format:   Servo myName;
// Our servo's name is radarServo. Write the missing word.
// ------------------------------------------------------------
____ radarServo;

// ------------------------------------------------------------
// STEP 6   (HINT STEP - write the line yourself)
// HINT: A list (called an array) holds many numbers under
// one name. We need a list to remember the distance at each angle.
//   Format:   int name[size];
//   Example:  int scores[10];     (a list of 10 whole numbers)
// Call the list distances and make its size NUM_POINTS.
// ------------------------------------------------------------


// ------------------------------------------------------------
// Measure the distance in cm (999 = nothing found)
// ------------------------------------------------------------
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // ----------------------------------------------------------
  // STEP 7
  // HINT: To send the sound, we turn the TRIG pin ON.
  //   digitalWrite(pin, state) turns a pin ON (HIGH) or OFF (LOW).
  // ----------------------------------------------------------
  digitalWrite(TRIG_PIN, ____);

  // ----------------------------------------------------------
  // STEP 8
  // HINT: Leave the pin on for a tiny moment: 10 microseconds.
  //   delayMicroseconds(time) pauses the Arduino.
  //   (A microsecond is a millionth of a second. Very small!)
  // ----------------------------------------------------------
  delayMicroseconds(____);

  // ----------------------------------------------------------
  // STEP 9   (HINT STEP - write the line yourself)
  // HINT: Now turn TRIG_PIN OFF again.
  // It is the same command as STEP 7, but with LOW.
  // ----------------------------------------------------------


  // ----------------------------------------------------------
  // STEP 10
  // HINT: pulseIn() is a stopwatch. It waits for a pin to switch
  // on, and tells us how many microseconds it stayed on.
  // The ECHO pin switches ON (HIGH) while the sound is travelling
  // and back. The 12000 means "give up after 12000 microseconds".
  //   Format: pulseIn(pin, HIGH or LOW, giveUpTime)
  // ----------------------------------------------------------
  long duration = pulseIn(ECHO_PIN, ____, 12000);

  if (duration == 0) return 999;   // no echo = nothing there

  // ----------------------------------------------------------
  // STEP 11   (HINT STEP - write the line yourself)
  // HINT: Turn the time into centimetres by dividing by 58.
  // (58 comes from the speed of sound. You don't need to know why!)
  //   Format:   int name = something / number;
  // Make a whole number called distance that equals duration / 58.
  // ----------------------------------------------------------


  return distance;
}

// ------------------------------------------------------------
// MATHS HELPERS (already written - you don't need to understand
// the maths, just use them!)
//   getX(angle, distance)  gives the X position on the screen
//   getY(angle, distance)  gives the Y position on the screen
// Give them an angle and how far from the centre, and they
// tell you where to draw on the OLED.
// ------------------------------------------------------------
int getX(int angle, int radius) {
  return CENTER_X + radius * cos(angle * PI / 180.0);
}

int getY(int angle, int radius) {
  return CENTER_Y - radius * sin(angle * PI / 180.0);
}

// ------------------------------------------------------------
// Draw the radar rings and base line (already written)
// ------------------------------------------------------------
void drawGrid() {
  display.drawLine(0, 63, 127, 63, SSD1306_WHITE);

  for (int r = 20; r <= 60; r += 20) {
    for (int a = 0; a <= 180; a += 4) {
      display.drawPixel(getX(a, r), getY(a, r), SSD1306_WHITE);
    }
  }
}

// ------------------------------------------------------------
// Draw one full radar frame
// ------------------------------------------------------------
void drawRadar(int sweepAngle, int currentDistance) {
  display.clearDisplay();
  drawGrid();

  // --------------------------------------------------------
  // STEP 12
  // HINT: This draws the sweep line from the centre of the radar
  // to the edge, pointing the way the sensor is facing.
  //   Format: drawLine(x1, y1, x2, y2, colour)
  //   It starts at the centre (CENTER_X, CENTER_Y).
  //   It ends at getX(angle, RADAR_RADIUS), getY(angle, RADAR_RADIUS).
  // The angle we want is sweepAngle. Put it in both blanks.
  // --------------------------------------------------------
  display.drawLine(CENTER_X, CENTER_Y,
                   getX(____, RADAR_RADIUS),
                   getY(____, RADAR_RADIUS),
                   SSD1306_WHITE);

  // Draw a blip for everything found so far
  for (int i = 0; i < NUM_POINTS; i++) {

    // ------------------------------------------------------
    // STEP 13
    // HINT: Only draw a blip if something was found closer than
    // the furthest distance the radar can show (MAX_DISTANCE).
    // Remember 999 means "nothing found".
    //   Format:   if (distances[i] < something) {
    // ------------------------------------------------------
    if (distances[i] < ____) {

      // Already written: the angle this blip was measured at
      int blipAngle = MIN_ANGLE + i * ANGLE_STEP;

      // ----------------------------------------------------
      // STEP 14   (HINT STEP - write the line yourself)
      // HINT: The distance is in centimetres (0 to 50) but the
      // radar is in pixels (0 to 60). map() converts one range
      // into the other.
      //   Format:   map(value, fromLow, fromHigh, toLow, toHigh)
      //   Example:  int x = map(5, 0, 10, 0, 100);   (x becomes 50)
      // Make a whole number called r.
      //   value     = distances[i]
      //   from      = 0 to MAX_DISTANCE
      //   to        = 0 to RADAR_RADIUS
      // ----------------------------------------------------


      // ----------------------------------------------------
      // STEP 15   (HINT STEP - write the line yourself)
      // HINT: Draw a filled dot. You used fillCircle in the
      // OLED lesson!
      //   Format: display.fillCircle(x, y, radius, colour);
      //   x      = getX(blipAngle, r)
      //   y      = getY(blipAngle, r)
      //   radius = 2
      //   colour = SSD1306_WHITE
      // ----------------------------------------------------

    }
  }

  // Already written: distance readout in the top left
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  if (currentDistance < MAX_DISTANCE) {
    display.print(currentDistance);
    display.print("cm");
  } else {
    display.print("--");
  }

  // ----------------------------------------------------------
  // STEP 16
  // HINT: Nothing shows on the screen until we send everything
  // we drew to it. You did this at the end of the OLED lesson.
  // The command is display.something();
  // ----------------------------------------------------------
  display.____();
}

void setup() {
  Serial.begin(9600);

  // ----------------------------------------------------------
  // STEP 17   (HINT STEP - write the lines yourself)
  // HINT: pinMode tells the Arduino if a pin sends a signal out
  // (OUTPUT) or listens for one (INPUT).
  //   Format:   pinMode(pin, mode);
  //   Example:  pinMode(LED_PIN, OUTPUT);
  // TRIG sends the sound out. ECHO listens for it coming back.
  // Write one line for TRIG_PIN and one for ECHO_PIN.
  // ----------------------------------------------------------


  // ----------------------------------------------------------
  // STEP 18
  // HINT: attach tells the Arduino which pin the servo is on.
  //   Format:   servoName.attach(pin);
  // ----------------------------------------------------------
  radarServo.____(SERVO_PIN);

  // Already written: start the OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while (true);   // OLED not found - stop here
  }

  // Already written: fill the list with 999 = "nothing here yet"
  for (int i = 0; i < NUM_POINTS; i++) {
    distances[i] = 999;
  }
}

// ------------------------------------------------------------
// Turn to one angle, measure, and redraw
// ------------------------------------------------------------
void scanAt(int index) {
  // Already written: work out the angle for this scan
  int angle = MIN_ANGLE + index * ANGLE_STEP;

  // ----------------------------------------------------------
  // STEP 19
  // HINT: write() turns the servo to an angle.
  //   Format:   servoName.write(angle);
  // Use the variable angle (made just above).
  // ----------------------------------------------------------
  radarServo.write(____);
  delay(60);   // give the servo time to get there

  // ----------------------------------------------------------
  // STEP 20   (HINT STEP - write the line yourself)
  // HINT: getDistance() measures and gives back a number.
  // Store that number in the list at position index.
  //   Format:   listName[position] = something;
  //   Example:  scores[3] = getScore();
  // The list is distances, the position is index.
  // ----------------------------------------------------------


  // Already written: print to the Serial Monitor and redraw the radar
  Serial.print(angle);
  Serial.print(" deg: ");
  Serial.print(distances[index]);
  Serial.println(" cm");

  drawRadar(angle, distances[index]);
}

// Already written: sweep one way, then back
void loop() {
  for (int i = 0; i < NUM_POINTS; i++) scanAt(i);
  for (int i = NUM_POINTS - 1; i >= 0; i--) scanAt(i);
}
