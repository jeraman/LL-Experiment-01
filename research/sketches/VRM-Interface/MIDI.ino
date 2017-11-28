#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Adafruit_BLEMIDI.h"
#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.7.0"

// This app was tested on iOS with the following apps:
//
// https://itunes.apple.com/us/app/midimittr/id925495245?mt=8
// https://itunes.apple.com/us/app/igrand-piano-free-for-ipad/id562914032?mt=8
//
// To test:
// - Run this sketch and open the Serial Monitor
// - Open the iGrand Piano Free app
// - Open the midimittr app on your phone and under Clients select "Adafruit Bluefruit LE"
// - When you see the 'Connected' label switch to the Routing panel
// - Set the Destination to 'iGrand Piano'
// - Switch to the iGrand Piano Free app and you should see notes playing one by one

// Create the bluefruit object, either software serial...uncomment these lines

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                              BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);


/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

Adafruit_BLEMIDI midi(ble);

bool isConnected = false;




void sendFeedbackMIDI (float value)
{
  value = floor(value*127);
  //0x90 = Channel 1 MIDI Note On
  midi.send(0x90, value, 0x64);
}


void sendPhasingMIDI (float value)
{
  value = floor(value*127);
  //0x80 = Channel 1 MIDI Note Off
  midi.send(0x80, value, 0x64);
}


void sendButtonPressMIDI ()
{
 //0xC0 = Channel 1 Program Change
  midi.send(0xC0, 0); 
}


void sendButtonPressTwiceMIDI ()
{
 //0xC0 = Channel 1 Program Change
  midi.send(0xC0, 1); 
}


void sendButtonHoldMIDI ()
{
 //0xC0 = Channel 1 Program Change
  midi.send(0xC0, 2); 
}


void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}


void connected(void)
{
  isConnected = true;
  Serial.println("CONNECTED!");
  delay(500);

}


void disconnect(void)
{
  Serial.println("disconnected...");
  isConnected = false;
}


void BleMidiRX(uint16_t timestamp, uint8_t status, uint8_t byte1, uint8_t byte2)
{
  Serial.print("[MIDI ");
  Serial.print(timestamp);
  Serial.print(" ] ");

  Serial.print(status, HEX); Serial.print(" ");
  Serial.print(byte1 , HEX); Serial.print(" ");
  Serial.print(byte2 , HEX); Serial.print(" ");

  Serial.println();
}


void setupMIDI()
{
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));

  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() )
      error(F("Couldn't factory reset"));
  }

  //ble.sendCommandCheckOK(F("AT+uartflow=off"));
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* Set BLE callbacks */
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnect);

  // Set MIDI RX callback
  midi.setRxCallback(BleMidiRX);

  Serial.println(F("Enable MIDI: "));
  if ( ! midi.begin(true) )
    error(F("Could not enable MIDI"));

  ble.verbose(false);
  Serial.println(F("Waiting for a connection..."));
}


void loopMIDI(void)
{
  // interval for each scanning ~ 500ms (non blocking)
  ble.update(500);

  // bail if not connected
  if (! isConnected)
    return;

  /*
    Serial.print("Sending pitch ");
    Serial.println(current_note, HEX);

    // send note on
    midi.send(0x90, current_note, 0x64);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);

    // send note off
    midi.send(0x80, current_note, 0x64);
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(500);

    // increment note pitch
    current_note++;

    // only do one octave
    if(current_note > 72)
    current_note = 60;
  */
}
