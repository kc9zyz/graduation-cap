#include <SPI.h>
#include <RH_NRF24.h>
#include "FastLED.h"
#define NUM_V_LEDS 17
#define DATA_PIN 6

CRGB leds[NUM_V_LEDS];
RH_NRF24 nrf24;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_V_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  //Start Radiohead library
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");

}

void loop() {
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (nrf24.recv(buf, &len))
  {
    if(len == 27)
    {
      memcpy((uint8_t*)&leds[0],buf,len);
      //FastLED.show(); 
    }
    else
    {
      memcpy((uint8_t*)&leds[9],buf,len);
      FastLED.show(); 
    }

  }
}
