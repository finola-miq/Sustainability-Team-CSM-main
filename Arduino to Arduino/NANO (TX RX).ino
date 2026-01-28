#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9
#define SS_1_PIN        2
#define SS_2_PIN        3
#define SS_3_PIN        4

#define OUTPUT_PIN_1    A0  // Pin for Reader 1
#define OUTPUT_PIN_2    A1  // Pin for Reader 2
#define OUTPUT_PIN_3    A2  // Pin for Reader 3

#define NR_OF_READERS   3

byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN};
MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instances

// UIDs for the green and red cards
byte greenCardUIDs[][4] = { 
    {0x43, 0x5E, 0x44, 0x10}, 
    {0x29, 0x7A, 0xC5, 0x4A} 
};

byte redCardUIDs[][4] = { 
    {0xA3, 0x5A, 0xBB, 0x10}, 
    {0xD3, 0xF7, 0x43, 0x10} 
};

// Store the last detected card state for each reader
char lastCard[NR_OF_READERS] = {'U', 'U', 'U'};

void setup() {
    Serial.begin(9600);
    while (!Serial);

    SPI.begin();

    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
        mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
        Serial.print(F("Reader "));
        Serial.print(reader + 1);
        Serial.print(F(": "));
        mfrc522[reader].PCD_DumpVersionToSerial();
    }

    pinMode(OUTPUT_PIN_1, OUTPUT);  
    pinMode(OUTPUT_PIN_2, OUTPUT);  
    pinMode(OUTPUT_PIN_3, OUTPUT);  

    digitalWrite(OUTPUT_PIN_1, LOW); 
    digitalWrite(OUTPUT_PIN_2, LOW);
    digitalWrite(OUTPUT_PIN_3, LOW);
}

void loop() {
    checkForCard(mfrc522[0], "Reader 1", OUTPUT_PIN_1, 0);
    checkForCard(mfrc522[1], "Reader 2", OUTPUT_PIN_2, 1);
    checkForCard(mfrc522[2], "Reader 3", OUTPUT_PIN_3, 2);
}

void checkForCard(MFRC522 &mfrc522, String readerName, int outputPin, int index) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print(readerName + " has detected a card. UID: ");
        dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
        Serial.println();

        char detectedCard = 'U';  // Default to Unknown

        if (isMatchingCard(mfrc522.uid.uidByte, greenCardUIDs, sizeof(greenCardUIDs) / 4)) {
            detectedCard = 'G';  // Green card detected
            digitalWrite(outputPin, HIGH);  // Turn pin high for Green card
            Serial.println(readerName + " detected a Green card. Pin is HIGH.");
        } else if (isMatchingCard(mfrc522.uid.uidByte, redCardUIDs, sizeof(redCardUIDs) / 4)) {
            detectedCard = 'R';  // Red card detected
            digitalWrite(outputPin, LOW);   // Turn pin low for Red card
            Serial.println(readerName + " detected a Red card. Pin is LOW.");
        } else {
            Serial.println("Unknown card detected.");
            detectedCard = 'U';  // Unknown card detected
        }

        // Only send data if a new card is detected
        if (detectedCard != lastCard[index]) {
            Serial.print(detectedCard);  // Send 'G', 'R', or 'U' to indicate the card type
            lastCard[index] = detectedCard;  // Update last detected card
        }

        int pinStatus = digitalRead(outputPin);
        Serial.print("Pin ");
        Serial.print(outputPin);
        Serial.print(" status: ");
        Serial.println(pinStatus == HIGH ? "HIGH" : "LOW");

        mfrc522.PICC_HaltA();
    }
}

bool isMatchingCard(byte *uid, byte validUIDs[][4], int numUIDs) {
    for (int i = 0; i < numUIDs; i++) {
        bool match = true;
        for (byte j = 0; j < 4; j++) {
            if (uid[j] != validUIDs[i][j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
