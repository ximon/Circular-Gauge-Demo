#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define pi 3.14159265359
#define pi2 2 * pi

#define afrMin 9
#define afrMax 19

float startAngleD, startAngle;
float endAngleD, endAngle;
int centerX, centerY, radius;
unsigned long renderTime = 0;

void setup()   {                
  Serial.begin(19200);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  setupGauge();
  drawGaugeFrame();
}

void setupGauge() {
  float centerD = 270; //Angle where the center of the gauge will be
  float widthD = 40; //Angle that the gauge will be wide
  
  startAngleD = centerD - widthD;
  endAngleD = centerD + widthD;

  centerX    = 63;//    Center of arc X (pixels)
  centerY    = 100;//    Center of arc Y (pixels)
  radius     = 65;//    Radious of arc (pixels)

  startAngle = startAngleD / 360 * pi2;
  endAngle = endAngleD / 360 * pi2;
}

void drawGaugeFrame() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  
  display.setCursor(0, 0);
  display.println("Rich");
  display.setCursor(100, 0);
  display.println("Lean");

  display.setTextSize(2);
  display.setCursor(45,15);
  display.println("AFR");

  drawGaugeLines(startAngle, endAngle, centerX, centerY, 65);
}

void loop() {
  drawGaugeData();
}

void drawGaugeData() {
  float value = getTestValue();
  float angle = scale(afrMin,afrMax,startAngleD,endAngleD,value); 
  char* afr = floatToString(value);

  drawValue(afr);
  drawNeedle(angle, startAngle, endAngle, centerX, centerY, radius, INVERSE);
  renderTime = millis() - renderTime;
  drawRenderTime();  
  display.display();
  drawRenderTime();
  renderTime = millis();
  drawNeedle(angle, startAngle, endAngle, centerX, centerY, radius, INVERSE); //erase the needle
  drawValue(afr);
}

void drawValue(char* value) {
  display.setTextColor(INVERSE);
  display.setTextSize(2);
  display.setCursor(34,50);
  display.println(value);
}

void drawRenderTime() {
  display.setTextColor(INVERSE);
  display.setCursor(0,57);
  display.setTextSize(1);
  display.println(renderTime);
}

float testVal = afrMin;
float inc = 0.2;
boolean increase = true;
float getTestValue() {
  if (testVal >= 19) increase = false;
  if (testVal <= 9) increase = true;
  if (increase) testVal+=inc;
  else testVal-=inc;
  return testVal;
}

char* floatToString(float value) {
  char afr[6];
  dtostrf( value, 5, 2, afr);       // float, width, precision, buffer
  return afr;
}

float scale(float inScaleMin, float inScaleMax, float outScaleMin, float outScaleMax, float value){
  return ((value - inScaleMin) / (inScaleMax - inScaleMin) * (outScaleMax-outScaleMin)) + outScaleMin;
}

void drawGaugeLines(float startAngle, float endAngle, float centerX, float centerY, int radius){
  drawArc(startAngle, endAngle, 150, centerX, centerY, radius + 30);
  drawArc(startAngle, endAngle, 110, centerX, centerY, radius - 1);
  drawArc(startAngle, endAngle, 110, centerX, centerY, radius - 4);
}

void drawNeedle(float angle, float startAngle, float endAngle, float centerX, float centerY, int radius, int color){
  float leftX = angleToXD(centerX, radius+1, angle - 5);
  float leftY = angleToYD(centerY, radius+1, angle - 5);

  float rightX = angleToXD(centerX, radius+1, angle + 5);
  float rightY = angleToYD(centerY, radius+1, angle + 5);

  float topX = angleToXD(centerX, radius+30, angle);
  float topY = angleToYD(centerY, radius+30, angle);

  display.fillTriangle(leftX,leftY,topX,topY,rightX,rightY,color);
}

void drawArc(float startAngle, float endAngle, float segments, int centerX, int centerY, int radius) {
  float resolution = (endAngle-startAngle)/segments; // Calculates steps in arc based on segments
  float x = centerX+radius*cos(startAngle); // Calculate start point of arc (X)
  float y = centerY+radius*sin(startAngle); // Calculate start point of arc (Y)
  display.writePixel(x,y,WHITE); // Place starting point of arc

  for (float angle = startAngle; angle < endAngle; angle += resolution) { // Sweep arc
      x = centerX+radius*cos(angle); // Calculate arc point (X)
      y = centerY+radius*sin(angle); // Calculate arc point (Y)
      display.writePixel(x,y,WHITE);
  }
}

float angleToXD(float centerX, float radius, float angleD) {
  float angle = (angleD / 360) * (pi2);
  return centerX+radius*cos(angle); // Calculate arc point (X)
}
float angleToYD(float centerY, float radius, float angleD) {
  float angle = (angleD / 360) * (pi2);
  return centerY+radius*sin(angle); // Calculate arc point (Y)
}


