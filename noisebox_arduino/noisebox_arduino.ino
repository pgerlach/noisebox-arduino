
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

int buttonState = 0;
int i;

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
  int i;
  for (i=0; i<nbButtons; ++i) {
    buttonState = debounce(tab[i].pin);
    if (buttonState == HIGH && buttonState != tab[i].previousState) {
        Serial.println(tab[i].txt);
    }
    tab[i].previousState = buttonState;
  }
}

