const int greenLedPin = 53;  // Pin for the green LED
const int redLedPin = 51;    // Pin for the red LED

char lastDetectedColor = 'U';  // Variable to store the last detected color

void setup() {
    Serial.begin(9600);  // Initialize serial communication
    pinMode(greenLedPin, OUTPUT);  // Set pin 53 as an output for green LED
    pinMode(redLedPin, OUTPUT);    // Set pin 51 as an output for red LED
    digitalWrite(greenLedPin, LOW);  // Ensure green LED is off initially
    digitalWrite(redLedPin, LOW);    // Ensure red LED is off initially
}

void loop() {
    if (Serial.available()) {
        char data = Serial.read();  // Read data from Nano

        // Only update LEDs if the detected color is different from the last detected color
        if (data != lastDetectedColor) {
            if (data == 'G') {
                // Handle Green card detected
                Serial.println("Green card detected");
                digitalWrite(greenLedPin, HIGH);  // Turn on the green LED
                digitalWrite(redLedPin, LOW);     // Ensure the red LED is off
            } else if (data == 'R') {
                // Handle Red card detected
                Serial.println("Red card detected");
                digitalWrite(greenLedPin, LOW);   // Ensure the green LED is off
                digitalWrite(redLedPin, HIGH);    // Turn on the red LED
            } else if (data == 'U') {
                // Handle Unknown card detected
                Serial.println("Unknown card detected");
                digitalWrite(greenLedPin, LOW);   // Ensure the green LED is off
                digitalWrite(redLedPin, LOW);     // Ensure the red LED is off
            }

            // Update the last detected color
            lastDetectedColor = data;
        }
    }
}
