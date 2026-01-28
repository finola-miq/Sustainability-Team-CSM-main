/**
 // Enable current reader by pulling its SS pin LOW
    digitalWrite(ssPins[reader], LOW);
*/
 
#include <SPI.h>
#include <MFRC522.h>
 
#define RST_PIN         9          // Reset pin remains at 9
#define SS_1_PIN        2          // RFID Reader 0 SS on pin 2
#define SS_2_PIN        3          // RFID Reader 1 SS on pin 3
#define SS_3_PIN        4          // RFID Reader 2 SS on pin 4
#define SS_4_PIN        5          // RFID Reader 3 SS on pin 5
 
#define NR_OF_READERS   4
 
// Array of slave select pins for the readers
byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN};
 
MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instances
 
/**
* Initialize.
*/
void setup() {
  Serial.begin(9600);               // Initialize serial communications with the PC
  while (!Serial);                  // Wait for the serial port to open (needed for some boards)
 
  SPI.begin();                      // Initialize SPI bus
 
  // Set all SS pins as outputs and disable all readers by default
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    pinMode(ssPins[i], OUTPUT);
    digitalWrite(ssPins[i], HIGH);
  }
 
  // Initialize each RFID reader
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Enable current reader for initialization
    digitalWrite(ssPins[reader], LOW);
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    // Disable it after initialization
    digitalWrite(ssPins[reader], HIGH);
 
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
    delay(100);  // slight delay between initializations
  }
}
 
/**
* Main loop.
*/
void loop() {
  // Check each RFID reader one at a time
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Disable all readers (set all SS pins HIGH)
    for (uint8_t i = 0; i < NR_OF_READERS; i++) {
      digitalWrite(ssPins[i], HIGH);
    }
    // Enable current reader by pulling its SS pin LOW
    digitalWrite(ssPins[reader], LOW);
 
    // Check if a new card is present on the active reader
    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Room "));
      Serial.print(reader);
      Serial.print(F(": Card#:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      // Halt PICC and stop encryption on PCD
      mfrc522[reader].PICC_HaltA();
      mfrc522[reader].PCD_StopCrypto1();
    }
    // Disable current reader before moving to the next
    digitalWrite(ssPins[reader], HIGH);
    delay(50);  // small delay before scanning the next reader
  }
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
