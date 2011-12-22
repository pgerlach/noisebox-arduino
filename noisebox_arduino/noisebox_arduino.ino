
const int debounceDelay = 10;

const int nbButtons = 5;

struct {
  int pin;
  const char* txt;
  int previousState;
} tab[nbButtons] = {
  { 2, "prev", LOW },
  { 3, "playpause", LOW },
  { 4, "next", LOW },
  { 5, "vminus", LOW },
  { 6, "vplus", LOW },
};

// buttons
int buttonState = 0;
int i;

// rfid
int  val = 0; 
char code[10];
char csum[2];
int bytesread = 0; 


void setup() {
  for (i=0; i<nbButtons; ++i) {
    pinMode(tab[i].pin, INPUT);
  }
  Serial.begin(9600);
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
  int i;
  for (i=0; i<nbButtons; ++i) {
    buttonState = debounce(tab[i].pin);
    if (buttonState == HIGH && buttonState != tab[i].previousState) {
        Serial.println(tab[i].txt);
    }
    tab[i].previousState = buttonState;
  }
  
  // rfid ( datasheet at http://www.seeedstudio.com/wiki/index.php?title=Electronic_brick_-_125Khz_RFID_Card_Reader)
  if (Serial.available() > 0)
  {
    if((val = Serial.read()) == 0x02) {
      bytesread = 0; 
      while(bytesread<10) {              // read 10 digit code 
        if( Serial.available() > 0) { 
          code[bytesread++] = Serial.read();         // add the digit           
        }
      }
      bytesread = 0;
      while (bytesread < 2) {
        if (Serial.available() > 0) {
          csum[bytesread++] = Serial.read();
        }
        // TODO check the checksum
      }
      bytesread = 0;
      while (bytesread < 1) {
        if (Serial.available() > 0) {
          val = Serial.read();
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

