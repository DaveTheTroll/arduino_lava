#include <FastLED.h>

#define LED_PIN   (2)
#define NUM_LEDS  (144)

CRGB leds[NUM_LEDS];
float heat[NUM_LEDS][2];
int cur = 1;

CRGB lava_color(int heat)
{
  if (heat < 92)
    return CRGB((heat*255)/92, 0, 0);           // Black to Red
  else if (heat < 184)
    return CRGB(255, ((heat-92)*255)/92, 0);    // Red to Yellow
  else
    return CRGB(255, 255, ((heat-184)*255)/73); // Yellow to White
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
    H = (9 * heat[i][cur] + heat[(i+1)%NUM_LEDS][cur])/10; // Weighted average of surroundings
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


  delay(1);
}
