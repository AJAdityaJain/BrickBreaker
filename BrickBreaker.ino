// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 9
#define OLED_DC     8
#define OLED_CS     10
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

const int size = 16;
const float dO = 0.5f;
const int brickw = 8;
const int brickh = 3;

const int playerY = 55;
int player = 32;

float x = 64;
float y = 50;
float dx = -dO;
float dy = -dO;

const uint8_t bricks[8][3] PROGMEM = {
  {0b11111110, 0b11111110,0b11111110},
  {0b11100110, 0b11111110,0b11011110},
  {0b10100110, 0b11111100,0b10011110},
  {0b00111010, 0b11111110,0b01110010},
  {0b01011000, 0b11111110,0b01101000},
  {0b00110010, 0b11001100,0b00110010},
  {0b01010100, 0b10101010,0b01010100},
  {0b00000000, 0b00000000,0b00000000}
};
uint8_t bricklist[] = {
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  0,0,0,0,0,0,0,7,7,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,7,7,0,0,0,0,0,0,0,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
};

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    for(;;);
  }
}

void loop() {
  int v = round(map(analogRead(A0),0,1023,0,255));
  if(v >= 41 && v <= 42){
    player += 1;
    if(player + size > SCREEN_WIDTH)player = SCREEN_WIDTH- size;
  }
  else if( v >= 22 && v <= 23){
    player -= 1;
    if(player < 0)player = 0;
  }
  if((y +dy)>= 64){
    delay(3000);
    testfillrect();
    display.clearDisplay();
      display.setTextSize(4);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F(" GAME OVER"));
    display.display();
    for(;;);
  }

  if((x+dx) >= 128 || (x+dx) < 0)dx *= -1;
  if((y+dy) < 0)dy *= -1;
  if(y >= playerY && y <= playerY+1 && x >= player && x <=player+size) {
    float theta = (x-(player+size/2))*PI/20;
    dx = dO * sin(theta);
    dy = dO * -cos(theta);
  }
  x += dx;
  y += dy;


  if(y < 16){
    int bx = floor(x/brickw);
    int by = floor(y/(brickh+2));
    int i = bx+ (by*16);
    if(bricklist[i] < 7){
      bricklist[i] ++;
      if(x <= bx*8 || x >= (bx+1)*8) dx *= -1;
      if(y <= by*8 || y >= (by+1)*8) dy *= -1;
    }
  }

  display.clearDisplay();

  for(int i = 0 ; i < 64; i++) {
    if(bricklist[i] < 7)
      display.drawBitmap((i % 16)*brickw, floor(i/16)*(brickh+2),  bricks[bricklist[i]], brickw,brickh, WHITE);
  }

  display.drawFastHLine(player,playerY+ 2 , size-4, SSD1306_WHITE);
  display.drawPixel(x,y, SSD1306_WHITE);
  display.display();  
  delay(5);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(100);
  }

  delay(1000);
}