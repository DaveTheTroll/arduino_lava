#include <FastLED.h>

#define LED_PIN   (2)
#define NUM_LEDS  (144)

CRGB leds[NUM_LEDS];
float heat[NUM_LEDS][2];
int cur = 1;

CRGB lava_color(int heat)
{
  const int red_heat = 20;
  const int orange_heat = 180;
  const int yellow_heat = 230;
  const int white_heat = 255;

  const int red_red      = 150;
  const int orange_green = 75;
  const int yellow_green = 150;
  const int white_blue   = 100;
  
  if (heat < red_heat)
    return CRGB((heat * red_red) / red_heat, 0, 0);           // Black to Red
  else if (heat < orange_heat)
    return CRGB(red_red, ((heat - red_heat) * orange_green) / (orange_heat - red_heat), 0);    // Red to Orange
  else if (heat < yellow_heat)
    return CRGB(red_red, orange_green + ((heat - orange_heat) * (yellow_green - orange_green)) / (yellow_heat - orange_heat), 0);    // Orange to Yellow
  else 
    return CRGB(red_red, yellow_green, ((heat - yellow_heat) * white_blue) / (white_heat - yellow_heat)); // Yellow to White
}

void setup() {
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  memset(heat, 0, sizeof(heat));
  heat[50][cur] = 255;
  Serial.begin(9600);
  Serial.println("Hello LEDs!");
}

void cool(float & h)
{
  h -= random(100)/50.0f;
  if (h<0) h = 0;
}

void loop()
{
  float H;
  int other = 1-cur;
  for(int i=0; i<NUM_LEDS; i++)
  {
    H = (1 * heat[(i+NUM_LEDS-1)%NUM_LEDS][cur] + 18 * heat[i][cur] + 4 * heat[(i+1)%NUM_LEDS][cur])/23; // Weighted average of surroundings
    heat[i][other] = max(H, heat[i][cur]); // Heat is weighted average or own heat, whichever is higher, with 1 cooling
    cool(heat[i][other]);
    leds[i] = lava_color((int)heat[i][other]);
  }
  FastLED.show();

  cur = other;

  // New heat source
  if (random(80) == 0)
  {
    heat[random(NUM_LEDS)][cur] = random(256);
  }

  delay(5);
}
