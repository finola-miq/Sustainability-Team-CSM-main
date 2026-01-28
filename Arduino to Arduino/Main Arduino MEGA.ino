//RELAYS ON BOARD A and C
#define RELAY_A_R1 46 //Let's pretend it is Board A and name for relay = RELAY_A_R1 where A-name of the board and R1-Red LED number 1
#define RELAY_A_G1 48 //Let's pretend it is Board A and name for relay = RELAY_A_G1 where A-name of the board and G1-Green LED number 1
#define RELAY_A_R2 50
#define RELAY_A_G2 52
#define RELAY_C_G1 47
#define RELAY_C_R1 49
#define RELAY_C_G2 51
#define RELAY_C_R2 53


//RELAYS ON BOARD D
#define RELAY_D_G1 45
#define RELAY_D_R1 43
#define RELAY_D_G2 41
#define RELAY_D_R2 39


//INPUT PINS
#define RFID1_D_G 2 //RFID1_D_G means that rfid reader number 1 is located on board D and it manages green LED
#define RFID1_D_R 3 //RFID1_D_R means that rfid reader number 1 is located on board D and it manages red LED
#define RFID2_D_G 4
#define RFID2_D_R 5
#define RFID3_D_G 6
#define RFID3_D_R 7

#define RFID1_A_G 8
#define RFID1_A_R 9
#define RFID2_A_G 10
#define RFID2_A_G 11
#define RFID1_C_G 12
#define RFID1_C_G 13
#define RFID2_C_G 22
#define RFID2_C_G 24




void setup() 
{
  //start serial connection
  Serial.begin(9600);
  //configure pins as an input and enable the internal pull-up resistor

// INPUT PINS FOR BOARD D
  pinMode(RFID1_D_G, INPUT_PULLUP);
  pinMode(RFID1_D_R, INPUT_PULLUP);
  pinMode(RFID2_D_G, INPUT_PULLUP);
  pinMode(RFID2_D_R, INPUT_PULLUP);
  pinMode(RFID3_D_G, INPUT_PULLUP);
  pinMode(RFID3_D_R, INPUT_PULLUP);
// INPUT PINS FOR BOARD A and C
  pinMode(RFID1_A_G, INPUT_PULLUP);
  pinMode(RFID1_A_R, INPUT_PULLUP);
  pinMode(RFID2_A_G, INPUT_PULLUP);
  pinMode(RFID2_A_G, INPUT_PULLUP);
  pinMode(RFID1_C_G, INPUT_PULLUP);
  pinMode(RFID1_C_G, INPUT_PULLUP);
  pinMode(RFID2_C_G, INPUT_PULLUP);
  pinMode(RFID2_C_G, INPUT_PULLUP);

//OUTPINS FOR BOARD D
  pinMode(RELAY_D_G1, OUTPUT);
  pinMode(RELAY_D_R1, OUTPUT);
  pinMode(RELAY_D_G2, OUTPUT);
  pinMode(RELAY_D_R2, OUTPUT);
//OUTPINS FOR BOARD A and C
  pinMode(RELAY_A_R1, OUTPUT);
  pinMode(RELAY_A_G1, OUTPUT);
  pinMode(RELAY_A_R2, OUTPUT);
  pinMode(RELAY_A_G2, OUTPUT);
  pinMode(RELAY_C_G1, OUTPUT);
  pinMode(RELAY_C_R1, OUTPUT);
  pinMode(RELAY_C_G2, OUTPUT);
  pinMode(RELAY_C_R2, OUTPUT);

 //RELAYS AND LEDs TEST
  //TESTING BOARD D
  digitalWrite(RELAY_D_G1, LOW);
  digitalWrite(RELAY_D_R1, LOW);
  digitalWrite(RELAY_D_G2, LOW);
  digitalWrite(RELAY_D_R2, LOW);
  delay(1000);
  digitalWrite(RELAY_D_G1, HIGH);
  digitalWrite(RELAY_D_R1, HIGH);
  digitalWrite(RELAY_D_G2, HIGH);
  digitalWrite(RELAY_D_R2, HIGH);
  delay(2000);
  //TESTING BOARD A and C
  // digitalWrite(RELAY_A_R1, HIGH);
  // digitalWrite(RELAY_A_G1, HIGH);
  // digitalWrite(RELAY_A_R2, HIGH);
  // digitalWrite(RELAY_A_G2, HIGH);
  // digitalWrite(RELAY_C_G1, HIGH);
  // digitalWrite(RELAY_C_R1, HIGH);
  // digitalWrite(RELAY_C_G2, HIGH);
  // digitalWrite(RELAY_C_R2, HIGH);
  // delay(1000);
  // digitalWrite(RELAY_A_R1, LOW);
  // digitalWrite(RELAY_A_G1, LOW);
  // digitalWrite(RELAY_A_R2, LOW);
  // digitalWrite(RELAY_A_G2, LOW);
  // digitalWrite(RELAY_C_G1, LOW);
  // digitalWrite(RELAY_C_R1, LOW);
  // digitalWrite(RELAY_C_G2, LOW);
  // digitalWrite(RELAY_C_R2, LOW);
  // delay(1000);
}

void loop() {
  //print out the value of the pushbutton

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:

   //GREEN
  if (RFID1_D_G == 1) 
  {
    Serial.print("RFID1_D_G pin 53: ");
    Serial.println(RFID1_D_G);
    digitalWrite(53, HIGH);
  }
  else if (RFID1_D_G == 0) 
  {
    Serial.print("RFID1_D_G pin 53: ");
    Serial.println(RFID1_D_G);
    digitalWrite(53, LOW); 
  } 
    
    
  
  //RED
  if (RFID1_D_R == 1) {
    digitalWrite(51, HIGH);
    } 
    else if (RFID1_D_R == 0) { 
    digitalWrite(51, LOW); 
  }  


}
