/*
=========================================================
                OLED CHARACTER MOVEMENT
=========================================================

GOAL

Use the joystick to move a character around the OLED
screen.

The joystick controls:

Left
Right
Up
Down

Pressing the joystick button changes the character.

---------------------------------------------------------
WHAT YOU WILL LEARN

• Reading analogue inputs
• Using if statements
• Updating variables
• Drawing shapes
• Keeping an object on the screen

=========================================================
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ------------------------------------------------------
// Joystick Pins
// ------------------------------------------------------

const int JOY_X = A0;
const int JOY_Y = A1;
const int BUTTON = 2;


// ------------------------------------------------------
// Player Variables
// ------------------------------------------------------

int playerX = 60;
int playerY = 30;

int oldX = playerX;
int oldY = playerY;

int character = 0;

bool lastButton = HIGH;


// ------------------------------------------------------
// Draw the Player
// ------------------------------------------------------

void drawPlayer(int x, int y, uint16_t colour)
{

  switch (character)
  {

    case 0:
      display.fillRect(x, y, 6, 6, colour);
      break;

    case 1:

      display.drawCircle(x + 3, y + 3, 3, colour);

      if (colour == SSD1306_WHITE)
      {
        display.drawPixel(x + 2, y + 2, colour);
        display.drawPixel(x + 4, y + 2, colour);
        display.drawLine(x + 2, y + 4, x + 4, y + 4, colour);
      }

      break;

    case 2:
      display.fillCircle(x + 3, y + 3, 3, colour);
      break;
  }
}


// ------------------------------------------------------
// Setup
// ------------------------------------------------------

void setup()
{

  pinMode(BUTTON, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.display();

}


// ------------------------------------------------------
// Main Loop
// ------------------------------------------------------

void loop()
{

  //------------------------------------------------------
  // STEP 1
  //
  // Save the player's current position.
  //
  // Hint:
  // The old position will need to be erased before the
  // player is drawn in the new position.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 2
  //
  // Read the joystick.
  //
  // Create TWO variables:
  //
  // xValue
  // yValue
  //
  // Hint:
  //
  // analogRead(...)
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 3
  //
  // Move LEFT.
  //
  // If the joystick is pushed left enough,
  // decrease playerX.
  //
  // Hint:
  //
  // if(...)
  // {
  //     playerX--;
  // }
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 4
  //
  // Move RIGHT.
  //
  // Increase playerX.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 5
  //
  // Move UP.
  //
  // Change playerY.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 6
  //
  // Move DOWN.
  //
  // Change playerY.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 7
  //
  // Keep the player on the screen.
  //
  // Hint:
  //
  // constrain(...)
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 8
  //
  // Read the joystick button.
  //
  // Store it in a variable called:
  //
  // button
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 9
  //
  // If the button has JUST been pressed,
  // change the character.
  //
  // character++;
  //
  // There are THREE characters.
  //
  // Make sure it loops back to 0.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 10
  //
  // Save the current button state.
  //
  // Hint:
  //
  // lastButton = ...
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 11
  //
  // Erase the OLD player.
  //
  // Hint:
  //
  // drawPlayer(...)
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 12
  //
  // Draw the NEW player.
  //------------------------------------------------------



  //------------------------------------------------------
  // STEP 13
  //
  // Show the new frame on the OLED.
  //
  // Hint:
  //
  // display.display();
  //------------------------------------------------------



  delay(20);

}