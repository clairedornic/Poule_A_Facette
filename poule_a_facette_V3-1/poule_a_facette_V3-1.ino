#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WebServer.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define NUMPIN 12
#define NUMPIXELS 16
#define PIN_ANALOG 32
#define POTAR_ANALOG 33

const char *ssid = "your_wifi";
const char *password = "your_password_wifi";
WebServer server(80);

int pattern[NUMPIXELS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

int val_analog,val_potar_analog;
bool min_is_first = true;
bool max_is_first = true;

int palette_colors;
int colors_lsd[NUMPIXELS][3] = {{0, 235, 64},{180, 235, 0},{235, 181, 0},{235, 91, 1},{235, 1, 1},{165, 1, 235},{1, 28, 235},{0, 207, 235}};
int colors_uwu[NUMPIXELS][3] = {{240, 228, 66},{247, 144, 59},{211, 54, 182},{95, 40, 184},{38, 117, 227},{22, 173, 223},{20, 204, 164},{43, 243, 63}};
int colors_sunshine[NUMPIXELS][3] = {{74, 94, 130},{87, 173, 239},{54, 241, 205},{19, 196, 163},{247, 179, 43},{254, 95, 85},{228, 87, 46},{112, 48, 31}};
int colors_autumn[NUMPIXELS][3] = {{136, 159, 4},{198, 201, 3},{236, 221, 9},{234, 180, 31},{252, 136, 34},{253, 101, 13},{224, 56, 0},{196, 57, 9}};
int colors_winter[NUMPIXELS][3] = {{242, 145, 189},{248, 58, 144},{199, 26, 173},{114, 9, 183},{58, 12, 163},{67, 97, 238},{76, 201, 240},{142, 222, 246}};

Adafruit_NeoPixel leds(NUMPIXELS, NUMPIN, NEO_GRB + NEO_KHZ800);

//Setup leds
//Setup WiFi connection
//Setup web route
//Setup website on local IP
void setup() {
  
  Serial.begin(9600);
  
  WiFi.persistent(false);
  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");

  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(100);
  }

  Serial.println("\n");
  Serial.println("Connexion etablie!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/lsd", change_palette_lsd);
  server.on("/uwu", change_palette_uwu);
  server.on("/sunshine", change_palette_sunshine);
  server.on("/automnale", change_palette_autumn);
  server.on("/hivernale", change_palette_winter);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Serveur web actif!");

  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  leds.begin();
}

//Read value from mic for frequencies
//Read value from potentiometer for brightness
//Select the leds colors according to our choice
//Waiting for client communication to choose colors
//Setup brightness according to potentiometer value
void loop() {
  val_potar_analog = analogRead(POTAR_ANALOG);
  val_analog = analogRead(PIN_ANALOG);
  leds.setBrightness(val_potar_analog/16.05);
  switch ( palette_colors )
  {
     case 0:
        chooseTheLed(val_analog, colors_lsd);
        break;
     case 1:
        chooseTheLed(val_analog, colors_uwu);
        break;
      case 2:
        chooseTheLed(val_analog, colors_sunshine);
        break;
      case 3:
        chooseTheLed(val_analog, colors_autumn);
        break;
      case 4:
        chooseTheLed(val_analog, colors_winter);
        break;
     default:
        chooseTheLed(val_analog, colors_lsd);
        break;
  }
  delay(75);
  server.handleClient();
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

//Choose colors LSD and redirect to homepage
void change_palette_lsd()
{
    palette_colors = 0;
    server.sendHeader("Location","/");
    server.send(303);
}

//Choose colors UWU and redirect to homepage
void change_palette_uwu()
{
    palette_colors = 1;
    server.sendHeader("Location","/");
    server.send(303);
}

//Choose colors Sunshine and redirect to homepage
void change_palette_sunshine()
{
    palette_colors = 2;
    server.sendHeader("Location","/");
    server.send(303);
}

//Choose colors Autumn and redirect to homepage
void change_palette_autumn()
{
    palette_colors = 3;
    server.sendHeader("Location","/");
    server.send(303);
}

//Choose colors Winter and redirect to homepage
void change_palette_winter()
{
    palette_colors = 4;
    server.sendHeader("Location","/");
    server.send(303);
}

//Page not found
void handleNotFound()
{
    server.send(404, "text/plain", "404: Not found");
}

//Homepage for local website
void handleRoot()
{
    String page;
    page += "<!DOCTYPE html>";
    page += "<html lang='en'>";
        page += "<head>";
            page += "<meta charset='UTF-8'>";
            page += "<meta http-equiv='X-UA-Compatible' content='IE=edge'>";
            page += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
            page += "<link rel='preconnect' href='https://fonts.googleapis.com'>";
            page += "<link rel='preconnect' href='https://fonts.googleapis.com'>";
            page += "<link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>";
            page += "<link href='https://fonts.googleapis.com/css2?family=Mada:wght@400;600;700&display=swap' rel='stylesheet'>";
            page += "<title>Poule à facettes</title>";
            page += "<style>";
                page += "@font-face{font-family:Guyon;src:url('https://clairedornic.fr/assets/fonts/GuyonGazebo-Italic.woff2') format('woff2');font-style:italic}@font-face{font-family:Guyon;src:url('https://clairedornic.fr/assets/fonts/GuyonGazebo-Regular.woff2') format('woff2');font-style:normal}body{background:url('https://clairedornic.fr/assets/img/bg-petit.jpg');background-position:center;background-repeat:no-repeat;background-size:cover;position:relative;height:100vh;margin:0;padding:0;overflow:hidden}body::after{content:url('https://clairedornic.fr/assets/img/boule.svg');position:absolute;top:-15px;left:50%;transform:translateX(-50%)}h1{font-family:Guyon;color:#fff;font-size:65px;text-align:center;padding-top:140px;position:relative;width:fit-content;margin:auto}h1::before{content:url('https://clairedornic.fr/assets/img/feuille.svg');position:absolute;top:45px;left:-178px}h1::after{content:url('https://clairedornic.fr/assets/img/feuille-right.svg');position:absolute;top:45px;right:-178px}h2{font-family:Mada,sans-serif;width:fit-content;margin:auto;color:#fff;position:relative;margin-top:20px;font-size:22px}h2::before{content:url('https://clairedornic.fr/assets/img/etoile.svg');position:absolute;top:50%;left:-50px;transform:translateY(-50%)}h2::after{content:url('https://clairedornic.fr/assets/img/etoile.svg');position:absolute;top:50%;right:-50px;transform:translateY(-50%)}.choice{margin:60px 0;display:flex;align-items:center;flex-direction:column}.choice .row{display:inline-flex;justify-content:space-between;width:50%;margin-bottom:25px}.choice .row-100{justify-content:center}a{text-decoration:none;font-family:Guyon;color:#fff;font-size:80px;text-align:center;font-size:30px;padding:15px 10px;background:rgb(255 255 255 / 40%);border-radius:25px;width:200px;transition:all .2s linear}a:hover{background:rgba(255,255,255,.56);transition:all .2s linear}@media screen and (min-width:1400px){.choice .row{max-width:700px}}@media screen and (max-width:1000px){h1::before{transform:scale(.7);top:80px;left:-115px}h1::after{transform:scale(.7);top:80px;right:-115px}h2{width:50%;text-align:center}.choice .row{width:70%}}@media screen and (max-width:780px){h1{width:50%;line-height:68px;font-size:60px}h1::before{left:-75px}h1::after{right:-75px}.choice .row{width:80%}a{font-size:30px}}@media screen and (max-width:600px){body{height:unset;min-height:100vh}body::after{transform:translateX(-50%) scale(.7)}h1{padding-top:14vh}h2{width:65%}.choice .row{flex-direction:column;align-items:center;margin-bottom:0}a{margin-bottom:20px}}@media screen and (max-width:500px){body::after{top:-25px;transform:translateX(-50%) scale(.5)}h1{width:85%;font-size:35px;line-height:40px;padding-top:80px}h1::before{transform:scale(.4);top:60px;left:-60px}h1::after{transform:scale(.4);top:60px;right:-60px}h2{font-size:18px;width:70%}h2::before{transform:translateY(-50%) scale(.6);left:-25px}h2::after{transform:translateY(-50%) scale(.6);right:-25px}.choice{margin:35px 0;display:flex;align-items:center;flex-direction:column}a{padding:10px 10px;font-size:20px;width:135px;margin-bottom:15px}}";
            page += "</style>";
        page += "</head>"
        page += "<body>";
            page += "<div class='container'>";
                page += "<div class='title'>";
                    page += "<h1>Poule à Facettes</h1>";
                    page += "<h2>Dans quelle ambiance souhaitez-vous, vous mettre aujourd'hui ?</h2>";
                page += "</div>";
                page += "<div class='choice'>";
                        page += "<div class='row row-50'>";
                            page += "<a href='/hivernale' class='btn'>Hivernale</a>";
                            page += "<a href='/automnale' class='btn'>Automnale</a>";
                        page += "</div>";
                        page += "<div class='row row-50'>";
                            page += "<a href='/lsd' class='btn'>LSD</a>";
                            page += "<a href='/sunshine' class='btn'>Sunshine</a>";
                        page += "</div>";
                        page += "<div class='row row-100'>";
                            page += "<a href='/uwu' class='btn'>U <span>w</span> U</a>";
                        page += "</div>";
                page += "</div>";
            page += "</div>";
        page += "</body>";
    page += "</html>";
                                    
    server.setContentLength(page.length());
    server.send(200, "text/html", page);
}
