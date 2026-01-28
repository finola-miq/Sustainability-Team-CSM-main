// check multiple buttons and toggle LEDs

enum { Off = HIGH, On = LOW };

byte pinsLed [] = { A1, A2, A3, A4, A5 };
byte pinsBut [] = { 2, 3, 4, 5, 6 };
#define N_BUT   sizeof(pinsBut)

byte butState [N_BUT];

// -----------------------------------------------------------------------------
int
chkButtons ()
{
    for (unsigned n = 0; n < sizeof(pinsBut); n++)  {
        byte but = digitalRead (pinsBut [n]);

        if (butState [n] != but)  {
            butState [n] = but;

            delay (30);     // debounce

            if (On == but)
                return n;
        }
    }
    return -1;
}
// -----------------------------------------------------------------------------
void
setup ()
{
    Serial.begin (9600);

    for (unsigned n = 0; n < sizeof(pinsBut); n++)  {
        pinMode (pinsBut [n], INPUT_PULLUP);
        butState [n] = digitalRead (pinsBut [n]);
    }

    for (unsigned n = 0; n < sizeof(pinsLed); n++)  {
        digitalWrite (pinsLed [n], Off);
        pinMode      (pinsLed [n], OUTPUT);
    }
}
// -----------------------------------------------------------------------------
void
loop ()
{
    switch (chkButtons ())  {
    case 4:
      Serial.println("4");
        // digitalWrite (pinsLed [A5], ! digitalRead (pinsLed [A5]));
        break;
    case 3:
      Serial.println("3");
        // digitalWrite (pinsLed [A4], ! digitalRead (pinsLed [A4]));
        break;
    case 2:
      Serial.println("2");
        // digitalWrite (pinsLed [A3], ! digitalRead (pinsLed [A3]));
        break;

    case 1:
      Serial.println("1");
        // digitalWrite (pinsLed [A2], ! digitalRead (pinsLed [A2]));
        break;

    case 0:
      Serial.println("START");
        // digitalWrite (pinsLed [A1], ! digitalRead (pinsLed [A1]));
        break;
    }
}

