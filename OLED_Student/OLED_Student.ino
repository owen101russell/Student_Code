/*
====================================================
OLED GRAPHICS LAB
====================================================

Today you are learning how to draw on an OLED screen.

The screen is:

128 pixels wide
64 pixels tall

The top-left corner is:

(0, 0)

The bottom-right corner is:

(127, 63)


====================================================
YOUR CHALLENGE
====================================================

Complete the basic drawings below.

Then use the tools to create your OWN picture.

You can make:

- A robot
- A car
- A house
- A spaceship
- A face
- Your name
- A monster
- A logo
- Anything you want!


====================================================
IMPORTANT
====================================================

The OLED does NOT show your drawing immediately.

After you finish drawing, you need:

    display.display();

====================================================
*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ==================================================
// OLED SETUP
// ==================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


// ==================================================
// SETUP
// ==================================================

void setup() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while (true);
  }

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);


  // =================================================
  // CHALLENGE 1 — DRAW A PIXEL
  // =================================================

  /*
  Draw ONE pixel at:

      X = 20
      Y = 20

  HINT:

  display.drawPixel(  ,  , SSD1306_WHITE );
  */


  // YOUR CODE HERE



  // =================================================
  // CHALLENGE 2 — DRAW A LINE
  // =================================================

  /*
  Draw a line from:

      (10, 40)

  to:

      (50, 40)

  HINT:

  display.drawLine(
      x1,
      y1,
      x2,
      y2,
      SSD1306_WHITE
  );
  */


  // YOUR CODE HERE



  // =================================================
  // CHALLENGE 3 — DRAW A RECTANGLE
  // =================================================

  /*
  Draw a rectangle.

  Try:

      X = 60
      Y = 15
      Width = 30
      Height = 20

  HINT:

  display.drawRect(
      x,
      y,
      width,
      height,
      SSD1306_WHITE
  );
  */


  // YOUR CODE HERE



  // =================================================
  // CHALLENGE 4 — FILLED RECTANGLE
  // =================================================

  /*
  Draw a FILLED rectangle.

  Try changing the size.

  HINT:

  fillRect() works almost exactly
  like drawRect().

  */


  // YOUR CODE HERE



  // =================================================
  // CHALLENGE 5 — CIRCLE
  // =================================================

  /*
  Draw a circle.

  Try:

      Centre X = 100
      Centre Y = 45
      Radius = 10

  HINT:

  display.drawCircle(
      x,
      y,
      radius,
      SSD1306_WHITE
  );
  */


  // YOUR CODE HERE



  // =================================================
  // CHALLENGE 6 — TEXT
  // =================================================

  /*
  Display your name!

  First choose a text size.

  Example:

      display.setTextSize(1);

  Then choose where the text goes:

      display.setCursor(x, y);

  Then:

      display.print("YOUR NAME");

  */


  // YOUR CODE HERE



  // =================================================
  // SHOW THE DRAWING
  // =================================================

  display.display();
}


void loop() {

}