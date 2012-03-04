#include <SoftwareSerial.h>

SoftwareSerial rfidSerial(2, 3);


// buttons
struct {
  int pin;
  const char* txt;
  int previousState;
} buttons[] = {
  { 4, "prev", HIGH },
  { 5, "playpause", HIGH },
  { 6, "next", HIGH },
  { 7, "vminus", HIGH },
  { 8, "vplus", HIGH },
  { 0, NULL, HIGH }
};
int buttonState = 0;
const int debounceDelay = 10;

// leds
const struct {
  char letter;
  char pin;
  char val;
} leds[] = {
  { 'a', 9, HIGH },
  { 'b', 9, LOW },
  { 'c', 10, HIGH },
  { 'd', 10, LOW },
  { 'e', 11, HIGH },
  { 'f', 11, LOW },  
  { 0, 0, 0 }
};



// rfid
int  val = 0; 
char code[10];
char csum[2];
int bytesread = 0; 


void setup() {
  for (int i=0; buttons[i].pin ; ++i) {
    int pin = buttons[i].pin;
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH); // enable pullup resistor
  }
  for (int i=0; leds[i].pin; ++i) {
    int pin = leds[i].pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  Serial.begin(9600);
  rfidSerial.begin(9600);
}


int debounce(int pin) {
  boolean state;
  boolean previousState;

  previousState = digitalRead(pin);
  for (int counter = 0; counter < debounceDelay; counter++) {
    delay(1);
    state = digitalRead(pin);
    if (state != previousState) {
      counter = 0;
      previousState = state;
    }
  }
  return state;
}

void loop(){

  // buttons
  for (int i=0; buttons[i].pin ; ++i) {
    buttonState = debounce(buttons[i].pin);
    if (buttonState == LOW && buttonState != buttons[i].previousState) {
        Serial.println(buttons[i].txt);
    }
    buttons[i].previousState = buttonState;
  }
  
  // commands (leds) via serial
  while (Serial.available() > 0) {
    char c = Serial.read();
    for (int i=0; leds[i].letter ; ++i) {
      if (leds[i].letter == c) {
        digitalWrite(leds[i].pin, leds[i].val);
        break ;
      }
    }
  }
  
  // rfid ( datasheet at http://www.seeedstudio.com/wiki/index.php?title=Electronic_brick_-_125Khz_RFID_Card_Reader)
  if (rfidSerial.available() > 0)
  {
    if((val = rfidSerial.read()) == 0x02) {
      bytesread = 0; 
      while(bytesread<10) {              // read 10 digit code 
        if( rfidSerial.available() > 0) { 
          code[bytesread++] = rfidSerial.read();         // add the digit           
        }
      }
      bytesread = 0;
      while (bytesread < 2) {
        if (rfidSerial.available() > 0) {
          csum[bytesread++] = rfidSerial.read();
        }
        // TODO check the checksum
      }
      bytesread = 0;
      while (bytesread < 1) {
        if (rfidSerial.available() > 0) {
          val = rfidSerial.read();
          bytesread++;
          if (val == 0x03) {
            Serial.print("rfid "); Serial.println(code);
          }
        }
      }
      bytesread = 0;
    }
  }

}

