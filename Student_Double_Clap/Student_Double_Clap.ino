const int soundPin = 7;
const int ledPin = 13;

bool ledState = false;

int clapCount = 0;
unsigned long firstClapTime = 0;

const unsigned long doubleClapWindow = 1500;
const unsigned long debounceTime = 100;

unsigned long lastClap = 0;

void setup() {
  // Set the sound sensor as an INPUT

  // Set the LED as an OUTPUT

  // Make sure the LED starts OFF

  // Start Serial Monitor at 9600 baud
}

void loop() {
  // Read the sound sensor and store its value in soundState

  // Check if a clap has been detected AND enough time has passed
  // since the previous clap

  if (/* Check for a clap and debounce */) {

    // Remember the time this clap was detected


    // Check if this is the FIRST clap
    if (/* Check clapCount */) {

      // Record that one clap has happened

      // Save the time of the first clap

      // Print "First clap detected" to the Serial Monitor

    }

    // Check if this is the SECOND clap
    // Make sure it happened within doubleClapWindow
    else if (/* Check clapCount and time */) {

      // Toggle ledState so TRUE becomes FALSE
      // and FALSE becomes TRUE

      // Update the physical LED using ledState

      // Print "Double clap!" to the Serial Monitor

      // Print whether the LED is currently ON or OFF

      // Reset clapCount so the Arduino can detect
      // another double clap

    }
  }

  // Check if we have received one clap,
  // but the second clap took longer than doubleClapWindow

  if (/* Check for timeout */) {

    // Forget about the first clap
  }
}