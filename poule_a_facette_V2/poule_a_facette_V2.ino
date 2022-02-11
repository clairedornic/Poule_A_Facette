#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define NUMPIN 12
#define NUMPIXELS 16
#define PIN_ANALOG 4

int pattern[NUMPIXELS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

int val_analog,min_analog, max_analog;
bool min_is_first = true;
bool max_is_first = true;

int palette_colors;
int colors_lsd[NUMPIXELS][3] = {{0, 235, 64},{180, 235, 0},{235, 181, 0},{235, 91, 1},{235, 1, 1},{165, 1, 235},{1, 28, 235},{0, 207, 235}};
int colors_uwu[NUMPIXELS][3] = {{240, 228, 66},{247, 144, 59},{211, 54, 182},{95, 40, 184},{38, 117, 227},{22, 173, 223},{20, 204, 164},{43, 243, 63}};

Adafruit_NeoPixel leds(NUMPIXELS, NUMPIN, NEO_GRB + NEO_KHZ800);

//Setup leds and setup brightness
void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  leds.begin();
  leds.setBrightness(94);
}
 
//Read value from mic for frequencies
//Select the leds colors according to our choice
void loop() {
  val_analog = analogRead(PIN_ANALOG);
  switch ( palette_colors )
  {
     case 0:
        chooseTheLed(val_analog, colors_lsd);
        break;
     case 1:
        chooseTheLed(val_analog, colors_uwu);
        break;
     default:
        chooseTheLed(val_analog, colors_lsd);
        break;
  }
  delay(75);
}

//Light up leds according to frequencies
void chooseTheLed(int analog, int colors[NUMPIXELS][3]){  
  // Turn off every leds
  for(int i=0; i < NUMPIXELS; i++) {
    leds.setPixelColor(i, leds.Color(0, 0, 0));
  }
  
  // Select the led.s to light up
  if(analog < 2650){
    light_them_up(&leds, pattern[3], pattern[3 + NUMPIXELS/2], colors[3][0], colors[3][1], colors[3][2]);
  }
  if(analog > 2575 && analog < 2825){
    light_them_up(&leds, pattern[6], pattern[6 + NUMPIXELS/2], colors[6][0], colors[6][1], colors[6][2]);
  }
  if(analog > 2750 && analog < 3000){
    light_them_up(&leds, pattern[1], pattern[1 + NUMPIXELS/2], colors[1][0], colors[1][1], colors[1][2]);
  }
  if(analog > 2925 && analog < 3175){
    light_them_up(&leds, pattern[4], pattern[4 + NUMPIXELS/2], colors[4][0], colors[4][1], colors[4][2]);
  }
  if(analog > 3100 && analog < 3350){
    light_them_up(&leds, pattern[0], pattern[0 + NUMPIXELS/2], colors[0][0], colors[0][1], colors[0][2]);
  }
  if(analog > 3275 && analog < 3525){
    light_them_up(&leds, pattern[5], pattern[5 + NUMPIXELS/2], colors[5][0], colors[5][1], colors[5][2]);
  }
  if(analog > 3450 && analog < 3700){
    light_them_up(&leds, pattern[2], pattern[2 + NUMPIXELS/2], colors[2][0], colors[2][1], colors[2][2]);
  }
  if(analog > 3625){
    light_them_up(&leds, pattern[7], pattern[7 + NUMPIXELS/2], colors[7][0], colors[7][1], colors[7][2]);
  }
}

//Light up 2 leds at the same time
void light_them_up(Adafruit_NeoPixel *leds, int num_led_pannel_1, int num_led_pannel_2, int red, int green, int blue) {
  // Set the new color
  leds->setPixelColor(num_led_pannel_1, leds->Color(red, green, blue));
  leds->setPixelColor(num_led_pannel_2, leds->Color(red, green, blue));
  // Light up
  leds->show();
}
