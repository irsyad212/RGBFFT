#include <arduinoFFT.h>

#define SAMPLES 64

#define SAMPLING_FREQ 20000

#define MIC_PIN A0

double vReal[SAMPLES];

double vImag[SAMPLES];

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQ);

#include <FastLED.h>

#define NUM_LEDS 64
#define LED_PIN 6
#define COLOR_ORDER GRB
#define TYPE WS2812
#define BRIGHTNESS 16

CRGB leds[NUM_LEDS];

uint8_t red, green, blue;

void setup() {
  delay(1000);
  FastLED.addLeds<TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(MIC_PIN);
    vImag[i] = 0;
  }

  FFT.dcRemoval();
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);

  red = map(vReal[3], 0, 1000, 0, 255);
  green = map(vReal[22], 0, 1000, 0, 255);
  blue = map(vReal[55], 0, 1000, 0, 255);

  if(red >= 256){
    red = 255;
  }

  if(red <= -1){
    red = 0;
  }

  if(red >= 256){
    red = 255;
  }

  if(red <= -1){
    red = 0;
  }

  if(green >= 256){
    green = 255;
  }

  if(green <= -1){
    green = 0;
  }

  if(blue >= 256){
    blue = 255;
  }

  if(blue <= -1){
    blue = 0;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(red, green, blue);
  }

  FastLED.show();
}