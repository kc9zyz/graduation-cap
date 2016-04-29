#include <ArduinoJson.h>

/*
  Example sketch for the RFCOMM/SPP Bluetooth library - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/

#include <SPP.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>
#include <RH_NRF24.h>
RH_NRF24 nrf24;

#include "FastLED.h"

// How many leds in your strip?
#define NUM_G_LEDS 19
#define NUM_V_LEDS 17

#define NUM_LEDS NUM_G_LEDS + NUM_V_LEDS

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
CRGB ledsTemp[NUM_G_LEDS];


USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
SPP SerialBT(&Btd, "GRADCAP", "1444"); // This will set the name to the defaults: "Arduino" and the pin to "0000"
//SPP SerialBT(&Btd, "Lauszus's Arduino", "1234"); // You can also set the name and pin like so

bool firstMessage = true;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_G_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nSPP Bluetooth Library Started"));

  //Start Radiohead library
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}
String myString = "";

void loop() {
  static uint8_t anim;
  static uint8_t hue;
  static uint8_t sat;
  static uint8_t value;
  static struct CHSV color;
  Usb.Task(); // The SPP data is actually not send until this is called, one could call SerialBT.send() directly as well

  if (SerialBT.connected) {
    if (firstMessage) {
      firstMessage = false;
      SerialBT.println(F("Hello from Arduino")); // Send welcome message
      Serial.println(F("Hello from Arduino")); // Send welcome message
    }
    while (SerialBT.available())
    {
      char readByte;

      if ((readByte = SerialBT.read()) != '.')
      {
        myString += readByte;
        Serial.print(readByte);
      }
      else
      {
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(myString);
        anim = root["a"];
        hue = root["h"];
        sat = root["s"];
        value = root["v"];
        Serial.println(value);
        color = {hue, sat, value};

        //SerialBT.println(myString);
        myString = "";
      }
    }
  }
  else
    firstMessage = true;
  switch (anim)
  {
    case 1:
      anim1(CHSV(hue, sat, value));
      break;
    case 2:
      anim2(color);
      break;
    case 3:
      anim3(value);
      break;
    case 4:
      anim4(value);
      break;
    case 5:
      anim5(CHSV(color));
      break;
    case 6:
      anim6(color);
      break;
  }


}
void anim1(const struct CRGB & color)
{
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = color;
    showLeds();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(500);
  }
  showLeds();

}
void anim2(struct CHSV & color)
{
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CHSV(color);
    color.hue += 2;
    showLeds();
    // clear this led for the next time around the loop
    delay(100);
    for (int i = 0; i < NUM_LEDS; i++)
    {
      nscale8x3(leds[i].r, leds[i].g, leds[i].b, 250);
    }
  }
  showLeds();
}
void anim3(uint8_t value)
{


  for (int dot = 0; dot < NUM_LEDS; dot++) {
    if (dot % 2)
    {
      leds[dot] = CHSV(147, 255, 135);
    }
    else
    {
      leds[dot] = CHSV(147, 0, 135);
    }
    showLeds();
    // clear this led for the next time around the loop
    delay(100);
  }
}
void anim4(uint8_t value)
{
  for (int i = 0; i < 2; i++)
  {
    for (int dot = 0; dot < NUM_LEDS; dot++) {
      if (dot % 2)
      {
        if (i % 2)
        {
          leds[dot] = CHSV(147, 255, 135);
        }
        else
        {
          leds[dot] = CHSV(147, 0, 135);
        }
      }
      else
      {
        if (i % 2)
        {
          leds[dot] = CHSV(147, 0, 135);
        }
        else
        {
          leds[dot] = CHSV(147, 255, 135);
        }
      }
      showLeds();
      // clear this led for the next time around the loop
    }
    delay(250);
  }

}
void anim5(const struct CRGB & color)
{
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = color;
    showLeds();
    // clear this led for the next time around the loop
    for (int i = 0; i < NUM_LEDS; i++)
    {
      nscale8x3(leds[i].r, leds[i].g, leds[i].b, 190);
    }
    delay(80);
  }
  showLeds();
}
void anim6(struct CHSV & color)
{
  uint8_t val = color.val;
  for (int i = 0; i < val; i += 7)
  {
    color.val = i;
    for (int dot = 0; dot < NUM_LEDS; dot++) {
      leds[dot] = CHSV(color);
      showLeds();
    }
    delay(5);
  }
  delay(100);
  for (int i = val; i >= 0; i -= 7)
  {
    color.val = i;
    for (int dot = 0; dot < NUM_LEDS; dot++) {
      leds[dot] = CHSV(color);
      showLeds();
    }
    delay(5);
  }
  color.hue += 40;
  color.val = val;
}

/* Reorder the data to display the center of the G first, send the V
    portion using NRF24, show the LEDs, and then restore LED order in the G
*/
void showLeds()
{
  int i;
  //Invert the G LEDS
  for(i=0;i<NUM_G_LEDS;i++)
  {
    ledsTemp[(NUM_G_LEDS-1)-i] = leds[i];
  }
  for(i=0;i<NUM_G_LEDS;i++)
  {
    leds[i] = ledsTemp[i];
  }

  //Send the V leds over the NRF24
//  nrf24.send((uint8_t*)&leds[NUM_G_LEDS], 28);
//  nrf24.waitPacketSent();
//  nrf24.send((uint8_t*)(&leds[NUM_G_LEDS]+28), (NUM_V_LEDS*sizeof(CRGB) - 28));
  nrf24.send((uint8_t*)&leds[NUM_G_LEDS], 27);
  nrf24.waitPacketSent();
  delay(1);
  nrf24.send((uint8_t*)&leds[NUM_G_LEDS+9], 24);
  nrf24.waitPacketSent();
  FastLED.show();

  //Restore the G leds
  for(i=0;i<NUM_G_LEDS;i++)
  {
    ledsTemp[(NUM_G_LEDS-1)-i] = leds[i];
  }
  for(i=0;i<NUM_G_LEDS;i++)
  {
    leds[i] = ledsTemp[i];
  }

}

