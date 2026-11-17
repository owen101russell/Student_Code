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

     distance = time x speed of sound / 2
     (divide by 2 because the sound travels there AND back)

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
  This code has 16 STEPS. There are two kinds:

     BLANK STEPS      Fill in every ____ .
     HINT STEPS       No code is given. Read the hint and
                      WRITE THE LINE(S) YOURSELF.

  The code will not upload until every step is done.
  Look for "STEP" in the code below to find each one.

  ------------------------------------------------------------
  STEP HINTS
  ------------------------------------------------------------
  STEP 1   The servo library is called Servo.
  STEP 2   The servo signal wire is on pin 6.
  STEP 3   HINT STEP. Write two #define lines:
           TRIG_PIN is pin 7 and ECHO_PIN is pin 8.
  STEP 4   Make a servo object. Its name is radarServo.
  STEP 5   HINT STEP. Make a list (array) of whole numbers.
           Name: distances       Size: NUM_POINTS
           Example of a list:    int scores[10];
  STEP 6   HINT STEP. Send the ping with 3 lines:
           turn TRIG_PIN HIGH, wait 10 microseconds,
           turn TRIG_PIN LOW.
           Commands: digitalWrite(pin, state);
                     delayMicroseconds(time);
  STEP 7   pulseIn waits for a pin to change and times it.
           Do we time the ECHO pin going HIGH or LOW?
  STEP 8   HINT STEP. Make an int called distance.
           Sound travels 0.034 cm every microsecond, and the
           sound goes there AND back (divide by 2).
           Use the variable duration.
  STEP 9   The sweep line ends at:
              x = centre + radius * cos(angle)
              y = centre - radius * sin(angle)
           Which of cos or sin goes where? (x uses cos)
  STEP 10  Only draw a blip if the object is closer than the
           furthest distance the radar can show.
  STEP 11  HINT STEP. Make an int called r that turns the
           distance (0 to MAX_DISTANCE) into pixels
           (0 to RADAR_RADIUS) with map().
           Format: map(value, fromLow, fromHigh, toLow, toHigh)
           The value is distances[i].
  STEP 12  HINT STEP. Draw the blip with display.fillCircle().
           Format: fillCircle(x, y, radius, colour)
           x = CENTER_X + r * cos(a)
           y = CENTER_Y - r * sin(a)
           Radius 2, colour SSD1306_WHITE.
  STEP 13  TRIG sends a signal out. ECHO listens for one.
           Which is OUTPUT and which is INPUT?
  STEP 14  Connect the servo to SERVO_PIN.
  STEP 15  Turn the servo to the angle for this scan.
  STEP 16  HINT STEP. Call getDistance() and store the answer in
           the list distances at position index.
           Example: scores[3] = 10;

  ------------------------------------------------------------
  TESTING
  ------------------------------------------------------------
  1. Upload and open the Serial Monitor (9600). You should see
     angles and distances scrolling past.
  2. Put your hand in front of the sensor. The distance should
     drop. If it stays at 999, check your wiring and STEPS 3, 6, 7.
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
  2. Change ANGLE_STEP to 3 for a sharper picture.
     (You will also need to change NUM_POINTS: how many?)
  3. Add a buzzer or LED that turns on when something is
     closer than 10 cm.
  4. Make the blips bigger the closer the object is.
  ============================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// STEP 1: add the servo library
#include <____.h>

// ----- OLED settings (already written) -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// STEP 2: the servo signal wire pin
#define SERVO_PIN ____

// STEP 3: (HINT STEP) write the #define lines for TRIG_PIN and ECHO_PIN here


// ----- Radar settings (already written) -----
#define CENTER_X 64        // radar centre (bottom middle of the screen)
#define CENTER_Y 63
#define RADAR_RADIUS 60    // radar size in pixels
#define MAX_DISTANCE 50    // furthest distance shown (cm)

#define MIN_ANGLE 15       // sweep from 15 degrees...
#define MAX_ANGLE 165      // ...to 165 degrees
#define ANGLE_STEP 5       // measure every 5 degrees
#define NUM_POINTS 31      // (165 - 15) / 5 + 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// STEP 4: make a servo object called radarServo
____ radarServo;

// STEP 5: (HINT STEP) make the list called distances here


// ------------------------------------------------------------
// Measure the distance in cm (999 = nothing found)
// ------------------------------------------------------------
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // STEP 6: (HINT STEP) send the ping (3 lines) here


  // STEP 7: time how long the echo takes to come back
  long duration = pulseIn(ECHO_PIN, ____, 12000);

  if (duration == 0) return 999;   // no echo = nothing there

  // STEP 8: (HINT STEP) turn duration into a distance in cm here


  return distance;
}

// ------------------------------------------------------------
// Draw the radar rings and base line (already written)
// ------------------------------------------------------------
void drawGrid() {
  display.drawLine(0, 63, 127, 63, SSD1306_WHITE);

  for (int r = 20; r <= 60; r += 20) {
    for (int a = 0; a <= 180; a += 4) {
      float rad = a * PI / 180.0;
      display.drawPixel(CENTER_X + r * cos(rad), CENTER_Y - r * sin(rad), SSD1306_WHITE);
    }
  }
}

// ------------------------------------------------------------
// Draw one full radar frame
// ------------------------------------------------------------
void drawRadar(int sweepAngle, int currentDistance) {
  display.clearDisplay();
  drawGrid();

  // Already written: turn degrees into radians (the maths needs this)
  float rad = sweepAngle * PI / 180.0;

  // STEP 9: draw the sweep line from the centre outwards
  display.drawLine(CENTER_X, CENTER_Y,
                   CENTER_X + RADAR_RADIUS * ____(rad),
                   CENTER_Y - RADAR_RADIUS * ____(rad),
                   SSD1306_WHITE);

  // Draw a blip for everything found so far
  for (int i = 0; i < NUM_POINTS; i++) {

    // STEP 10: only draw if the distance is closer than the furthest the radar shows
    if (distances[i] < ____) {

      // Already written: the angle of this blip, in radians
      float a = (MIN_ANGLE + i * ANGLE_STEP) * PI / 180.0;

      // STEP 11: (HINT STEP) turn the distance into pixels (call it r) here


      // STEP 12: (HINT STEP) draw the blip here

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

  display.display();
}

void setup() {
  Serial.begin(9600);

  // STEP 13: set up the sensor pins
  pinMode(TRIG_PIN, ____);
  pinMode(ECHO_PIN, ____);

  // STEP 14: connect the servo to its pin
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
  int angle = MIN_ANGLE + index * ANGLE_STEP;

  // STEP 15: turn the servo to this angle
  radarServo.write(____);
  delay(60);   // give the servo time to get there

  // STEP 16: (HINT STEP) measure and store the distance in the list here


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
