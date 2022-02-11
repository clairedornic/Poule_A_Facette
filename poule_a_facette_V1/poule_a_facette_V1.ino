int ledBlue = 12;
int ledRed = 13;
int ledYellow = 27;
int ledGreen = 25;
int sound_digital = 0;
int sound_analog = 4;
int val_digital,val_analog, min_analog, max_analog;
bool min_is_first = true;
bool max_is_first = true;

//Setup different PIN
void setup(){
  Serial.begin(9600);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(sound_digital, INPUT);  
}

//Setup light to LOW 
//Read value from mic for frequencies
//Print minimum and maximum value from frequencies
void loop(){
  val_digital = digitalRead(sound_digital);
  val_analog = analogRead(sound_analog);
  digitalWrite (ledRed, LOW);
  digitalWrite (ledBlue, LOW);
  digitalWrite (ledGreen, LOW);
  digitalWrite (ledYellow, LOW);
  activateLed(val_analog);
  
  Serial.print(val_analog);
  Serial.print("\n");
  
  setMin(val_analog);
  setMax(val_analog);

  Serial.print(min_analog);
  Serial.print("\t");
  Serial.print(max_analog);
  Serial.print("\n");
}

//Set the minimum value of frequencies
void setMin(int val_analog) {
  // Check the first call
  if(min_is_first) {
    min_is_first = false;
    min_analog = val_analog;
  } else {
    if(min_analog > val_analog) {
      min_analog = val_analog;
    }
  }
}

//Set the maximum value of frequencies
void setMax(int val_analog) {
  // Check the first call
  if(max_is_first) {
    max_is_first = false;
    max_analog = val_analog;
  } else {
    if(max_analog < val_analog) {
      max_analog = val_analog;
    }
  }
}

//Light up blue led
void lightLedBlue() {
   digitalWrite (ledBlue, HIGH);
}

//Light up red led
void lightLedRed(){
  digitalWrite (ledRed, HIGH);
}

//Light up yellow led
void lightLedYellow() {
   digitalWrite (ledYellow, HIGH);
}

//Light up green led
void lightLedGreen(){
  digitalWrite (ledGreen, HIGH);
}

//Light up led according to frequencies
void activateLed(int analog){
  if(analog < 1750){
   lightLedBlue();
  }
  else if(analog > 1750 && analog < 1950){
   lightLedRed();
  }
  else if(analog > 2050 && analog < 2150){
   lightLedYellow();
  }
  else if(analog > 2150){
   lightLedGreen();
  }
}
