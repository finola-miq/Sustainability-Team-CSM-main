// function prototypes (optional, but helps readability)
void processSerialCommands();
void handleLEDCommand(String command);
bool checkGroupsActive();
bool checkAllGreen();
void updateSystemState();
void updateBlinking(unsigned long currentMillis);
void resetLEDs();
// *********************
// Define LED pins for each board and pair
// *********************

// Board A LED definitions:
#define LED_RED_A1   23  // Board A pair 1 red LED
#define LED_GREEN_A1 22  // Board A pair 1 green LED
#define LED_RED_A2   25  // Board A pair 2 red LED
#define LED_GREEN_A2 24  // Board A pair 2 green LED

// Board C LED definitions:
#define LED_RED_C1    27  // Board C pair 1 red LED
#define LED_GREEN_C1  26  // Board C pair 1 green LED
#define LED_RED_C2    29  // Board C pair 2 red LED
#define LED_GREEN_C2  28  // Board C pair 2 green LED

// Board D LED definitions:
#define LED_RED_D1    33  // Board D pair 1 red LED
#define LED_GREEN_D1  32  // Board D pair 1 green LED
#define LED_RED_D2    35  // Board D pair 2 red LED
#define LED_GREEN_D2  34  // Board D pair 2 green LED
#define LED_RED_D3    31  // Board D pair 3 red LED
#define LED_GREEN_D3  30  // Board D pair 3 green LED

// Board E LED definitions:
#define LED_RED_E1    37  // Board E pair 1 red LED
#define LED_GREEN_E1  36  // Board E pair 1 green LED
#define LED_RED_E2    39  // Board E pair 2 red LED
#define LED_GREEN_E2  38  // Board E pair 2 green LED

// Board F LED definitions:
#define LED_RED_F1    43  // Board F pair 1 red LED
#define LED_GREEN_F1  40  // Board F pair 1 green LED

// *********************
// State Machine Setup and Global Variables
// *********************

enum SystemState { 
  STATE_IDLE,              // Not all groups are active
  STATE_WAITING_FOR_GREEN, // All groups active but not all green LEDs are lit yet
  STATE_BLINKING           // All green LEDs are lit; blink sequence active
};

SystemState currentState = STATE_IDLE;

// For periodic state checks:
unsigned long lastStateCheckTime = 0;
const unsigned long stateCheckInterval = 100; // milliseconds

// Variables for non-blocking blinking using millis()
unsigned long blinkStartMillis = 0;        // when this blink burst began
const unsigned long blinkDuration = 2500;  // total blink time in ms (3 s)
unsigned long blinkPrevMillis = 0;
const unsigned long blinkInterval = 300;      // Blink every 500ms
int blinkCount = 0;
const int totalBlinks = 5;                    // Blink 5 times
bool blinkOn = false;                         // Current blinking phase

// Array of green LED pins for blinking (active-low: LOW = LED on)
int ledPins[10] = { LED_GREEN_A1, LED_GREEN_A2, 
                    LED_GREEN_C1, LED_GREEN_C2,
                    LED_GREEN_D1, LED_GREEN_D2, LED_GREEN_D3,
                    LED_GREEN_E1, LED_GREEN_E2, 
                    LED_GREEN_F1 };

// All LEDs (both red and green) in one array:
const int allLedPins[] = {
  // Board A
  LED_RED_A1, LED_GREEN_A1,
  LED_RED_A2, LED_GREEN_A2,
  // Board C
  LED_RED_C1, LED_GREEN_C1,
  LED_RED_C2, LED_GREEN_C2,
  // Board D
  LED_RED_D1, LED_GREEN_D1,
  LED_RED_D2, LED_GREEN_D2,
  LED_RED_D3, LED_GREEN_D3,
  // Board E
  LED_RED_E1, LED_GREEN_E1,
  LED_RED_E2, LED_GREEN_E2,
  // Board F
  LED_RED_F1, LED_GREEN_F1
};
const int numAllLeds = sizeof(allLedPins) / sizeof(allLedPins[0]);


// *********************
// Setup Function: Runs once at startup
// *********************
void setup() {
  // Initialize serial communications for four Nano boards:
  Serial.begin(9600);   // Nano A (USB/hardware Serial)
  Serial1.begin(9600);  // Nano C (Serial1)
  Serial2.begin(9600);  // Nano D (Serial2)
  Serial3.begin(9600);  // Nano E (Serial3)
  
  // Set up output pins for Board A LEDs:
  pinMode(LED_RED_A1, OUTPUT);
  pinMode(LED_GREEN_A1, OUTPUT);
  pinMode(LED_RED_A2, OUTPUT);
  pinMode(LED_GREEN_A2, OUTPUT);

  // Board C LEDs:
  pinMode(LED_RED_C1, OUTPUT);
  pinMode(LED_GREEN_C1, OUTPUT);
  pinMode(LED_RED_C2, OUTPUT);
  pinMode(LED_GREEN_C2, OUTPUT);

  // Board D LEDs:
  pinMode(LED_RED_D1, OUTPUT);
  pinMode(LED_GREEN_D1, OUTPUT);
  pinMode(LED_RED_D2, OUTPUT);
  pinMode(LED_GREEN_D2, OUTPUT);
  pinMode(LED_RED_D3, OUTPUT);
  pinMode(LED_GREEN_D3, OUTPUT);

  // Board E LEDs:
  pinMode(LED_RED_E1, OUTPUT);
  pinMode(LED_GREEN_E1, OUTPUT);
  pinMode(LED_RED_E2, OUTPUT);
  pinMode(LED_GREEN_E2, OUTPUT);

  // Board F LEDs:
  pinMode(LED_RED_F1, OUTPUT);
  pinMode(LED_GREEN_F1, OUTPUT);

  // Turn all LEDs RED initially
  resetLEDs();

  Serial.println("MEGA finished setup");
}

// *********************
// Main Loop: Runs continuously after setup()
// *********************
void loop() {
  // Always process incoming serial commands:
  processSerialCommands();
  
  unsigned long now = millis();
  
  // Periodically update the system state (non-blocking):
  if (now - lastStateCheckTime >= stateCheckInterval) {
    lastStateCheckTime = now;
    updateSystemState();
  }
  
  // If in the BLINKING state, update the blinking sequence non-blockingly:
  if (currentState == STATE_BLINKING) {
    updateBlinking(now);
  }
}


// *********************
// Serial Command Processing
// *********************
void processSerialCommands() {
  // Serial (USB to computer) - could be RESTART or RFID from Nano A
  if (Serial.available() > 0) {  
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    // Check if it's a system command (RESTART/RESET) first
    if (command == "RESTART" || command == "RESET") {
      Serial.println("Received RESTART from computer");
      handleLEDCommand(command);
    } else {
      // It's from Nano A
      Serial.print("From Nano A: ");
      Serial.println(command);
      handleLEDCommand(command);
    }
  }
  
  if (Serial1.available() > 0) {  
    String command = Serial1.readStringUntil('\n');
    command.trim();
    Serial.print("From Nano C: ");
    Serial.println(command);
    handleLEDCommand(command);
  }
  
  if (Serial2.available() > 0) {  
    String command = Serial2.readStringUntil('\n');
    command.trim();
    Serial.print("From Nano D: ");
    Serial.println(command);
    handleLEDCommand(command);
  }
  
  if (Serial3.available() > 0) {  
    String command = Serial3.readStringUntil('\n');
    command.trim();
    Serial.print("From Nano E: ");
    Serial.println(command);
    handleLEDCommand(command);
  }
}

// *********************
// Process Incoming LED Commands
// *********************
void handleLEDCommand(String command) {
  // Remove any extra whitespace from the command.
  command.trim();

  // Reset conditions: Support both RESET and RESTART commands.
  if (command == "RESET" || command == "RESTART") {
    Serial.println("Resetting LEDs and system state... please wait a second");
    resetLEDs();             // Turn all LEDs RED.
    currentState = STATE_IDLE; // Reset the system state.
    blinkCount = 0;            // Reset blink counter.
    blinkOn = false;           // Reset blinking phase.
    return;                  // Exit without processing further.
  }
  
  // --- Board A Commands ---
  if (command == "A1_RED") { 
    digitalWrite(LED_RED_A1, LOW);
    digitalWrite(LED_GREEN_A1, HIGH);
    Serial.println("A1_RED");
  } else if (command == "A1_GREEN") {
    digitalWrite(LED_RED_A1, HIGH);
    digitalWrite(LED_GREEN_A1, LOW);
    Serial.println("A1_GREEN");
  } else if (command == "A2_RED") {
    digitalWrite(LED_RED_A2, LOW);
    digitalWrite(LED_GREEN_A2, HIGH);
    Serial.println("A2_RED");
  } else if (command == "A2_GREEN") {
    digitalWrite(LED_RED_A2, HIGH);
    digitalWrite(LED_GREEN_A2, LOW);
    Serial.println("A2_GREEN");
  }
  // --- Board C Commands ---
  else if (command == "C1_RED") { 
    digitalWrite(LED_RED_C1, LOW);
    digitalWrite(LED_GREEN_C1, HIGH);
    Serial.println("C1_RED");
  } else if (command == "C1_GREEN") {
    digitalWrite(LED_RED_C1, HIGH);
    digitalWrite(LED_GREEN_C1, LOW);
    Serial.println("C1_GREEN");
  } else if (command == "C2_RED") {
    digitalWrite(LED_RED_C2, LOW);
    digitalWrite(LED_GREEN_C2, HIGH);
    Serial.println("C2_RED");
  } else if (command == "C2_GREEN") {
    digitalWrite(LED_RED_C2, HIGH);
    digitalWrite(LED_GREEN_C2, LOW);
    Serial.println("C2_GREEN");
  }
  // --- Board D Commands ---
  else if (command == "D1_RED") {
    digitalWrite(LED_RED_D1, LOW);
    digitalWrite(LED_GREEN_D1, HIGH);
    Serial.println("D1_RED");
  } else if (command == "D1_GREEN") {
    digitalWrite(LED_RED_D1, HIGH);
    digitalWrite(LED_GREEN_D1, LOW);
    Serial.println("D1_GREEN");
  } else if (command == "D2_RED") {
    digitalWrite(LED_RED_D2, LOW);
    digitalWrite(LED_GREEN_D2, HIGH);
    Serial.println("D2_RED");
  } else if (command == "D2_GREEN") {
    digitalWrite(LED_RED_D2, HIGH);
    digitalWrite(LED_GREEN_D2, LOW);
    Serial.println("D2_GREEN");
  } else if (command == "D3_RED") {
    digitalWrite(LED_RED_D3, LOW);
    digitalWrite(LED_GREEN_D3, HIGH);
    Serial.println("D3_RED");
  } else if (command == "D3_GREEN") {
    digitalWrite(LED_RED_D3, HIGH);
    digitalWrite(LED_GREEN_D3, LOW);
    Serial.println("D3_GREEN");
  }
  // --- Board E Commands ---
  else if (command == "E1_RED") {
    digitalWrite(LED_RED_E1, LOW);
    digitalWrite(LED_GREEN_E1, HIGH);
    Serial.println("E1_RED");
  } else if (command == "E1_GREEN") {
    digitalWrite(LED_RED_E1, HIGH);
    digitalWrite(LED_GREEN_E1, LOW);
    Serial.println("E1_GREEN");
  } else if (command == "E2_RED") {
    digitalWrite(LED_RED_E2, LOW);
    digitalWrite(LED_GREEN_E2, HIGH);
    Serial.println("E2_RED");
  } else if (command == "E2_GREEN") {
    digitalWrite(LED_RED_E2, HIGH);
    digitalWrite(LED_GREEN_E2, LOW);
    Serial.println("E2_GREEN");
  }
  // --- Board F Commands ---
  else if (command == "F1_RED") {
    digitalWrite(LED_RED_F1, LOW);
    digitalWrite(LED_GREEN_F1, HIGH);
    Serial.println("F1_RED");
  } else if (command == "F1_GREEN") {
    digitalWrite(LED_RED_F1, HIGH);
    digitalWrite(LED_GREEN_F1, LOW);
    Serial.println("F1_GREEN");
  }
}

// *********************
// Check if all LED groups (pairs) have been set (either LED active)
// *********************
bool checkGroupsActive() {
  bool groupA1 = (digitalRead(LED_RED_A1) == LOW || digitalRead(LED_GREEN_A1) == LOW);
  bool groupA2 = (digitalRead(LED_RED_A2) == LOW || digitalRead(LED_GREEN_A2) == LOW);
  bool groupC1 = (digitalRead(LED_RED_C1) == LOW || digitalRead(LED_GREEN_C1) == LOW);
  bool groupC2 = (digitalRead(LED_RED_C2) == LOW || digitalRead(LED_GREEN_C2) == LOW);
  bool groupD1 = (digitalRead(LED_RED_D1) == LOW || digitalRead(LED_GREEN_D1) == LOW);
  bool groupD2 = (digitalRead(LED_RED_D2) == LOW || digitalRead(LED_GREEN_D2) == LOW);
  bool groupD3 = (digitalRead(LED_RED_D3) == LOW || digitalRead(LED_GREEN_D3) == LOW);
  bool groupE1 = (digitalRead(LED_RED_E1) == LOW || digitalRead(LED_GREEN_E1) == LOW);
  bool groupE2 = (digitalRead(LED_RED_E2) == LOW || digitalRead(LED_GREEN_E2) == LOW);
  bool groupF1 = (digitalRead(LED_RED_F1) == LOW || digitalRead(LED_GREEN_F1) == LOW);
  return (groupA1 && groupA2 && groupC1 && groupC2 &&
          groupD1 && groupD2 && groupD3 &&
          groupE1 && groupE2 && groupF1);
}

// *********************
// Check if all green LEDs are lit (active-low: LOW means ON)
// *********************
bool checkAllGreen() {
  return (digitalRead(LED_GREEN_A1) == LOW &&
          digitalRead(LED_GREEN_A2) == LOW &&
          digitalRead(LED_GREEN_C1) == LOW &&
          digitalRead(LED_GREEN_C2) == LOW &&
          digitalRead(LED_GREEN_D1) == LOW &&
          digitalRead(LED_GREEN_D2) == LOW &&
          digitalRead(LED_GREEN_D3) == LOW &&
          digitalRead(LED_GREEN_E1) == LOW &&
          digitalRead(LED_GREEN_E2) == LOW &&
          digitalRead(LED_GREEN_F1) == LOW);
}

// *********************
// Update the system state (non-blocking)
// *********************
void updateSystemState() {
  unsigned long now = millis();

  // 1) If we're already in disco/blinking, check for timeout
  if (currentState == STATE_BLINKING) {
    if (now - blinkStartMillis >= blinkDuration) {
      // 3 seconds have passed -> stop disco
      resetLEDs();
      currentState = STATE_IDLE;
      return;   // skip the rest
    }
  }

  // 2) Otherwise, if all 10 groups are active+green, enter blinking
  bool allActive = checkGroupsActive();
  bool allGreen  = checkAllGreen();

  if (allActive && allGreen) {
    if (currentState != STATE_BLINKING) {
      currentState     = STATE_BLINKING;
      blinkStartMillis = now;     // start 3s window
      blinkPrevMillis  = now;     // reset toggle timer
      blinkOn          = false;   // begin with LEDs off
    }
  }
  // 3) If not blinking, update between IDLE and WAITING
  else if (currentState != STATE_BLINKING) {
    currentState = allActive ? STATE_WAITING_FOR_GREEN : STATE_IDLE;
  }
}
// *********************
// Blinking sequence
// *********************
void updateBlinking(unsigned long now) {
  // every blinkInterval ms, pick a fresh random on/off for each LED
  if (now - blinkPrevMillis >= blinkInterval) {
    blinkPrevMillis = now;

    for (int i = 0; i < numAllLeds; i++) {
      // random(0,2) returns either 0 or 1
      // active-low: LOW = ON, HIGH = OFF
      digitalWrite(allLedPins[i], random(0, 2) ? LOW : HIGH);
    }
  }
}

// *********************
// Reset all LED pins to off (active-low: HIGH means off)
// *********************
void resetLEDs() { 
// RED ON, GREEN OFF (active-low logic: LOW = ON, HIGH = OFF)
  digitalWrite(LED_RED_A1, LOW);    digitalWrite(LED_GREEN_A1, HIGH);
  digitalWrite(LED_RED_A2, LOW);    digitalWrite(LED_GREEN_A2, HIGH);

  digitalWrite(LED_RED_C1, LOW);    digitalWrite(LED_GREEN_C1, HIGH);
  digitalWrite(LED_RED_C2, LOW);    digitalWrite(LED_GREEN_C2, HIGH);

  digitalWrite(LED_RED_D1, LOW);    digitalWrite(LED_GREEN_D1, HIGH);
  digitalWrite(LED_RED_D2, LOW);    digitalWrite(LED_GREEN_D2, HIGH);
  digitalWrite(LED_RED_D3, LOW);    digitalWrite(LED_GREEN_D3, HIGH);

  digitalWrite(LED_RED_E1, LOW);    digitalWrite(LED_GREEN_E1, HIGH);
  digitalWrite(LED_RED_E2, LOW);    digitalWrite(LED_GREEN_E2, HIGH);

  digitalWrite(LED_RED_F1, LOW);    digitalWrite(LED_GREEN_F1, HIGH); 
}
