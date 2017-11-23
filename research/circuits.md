# Prototyping Circuitry
These are the steps to get the 'pedal' working:

1. Get started on Arduino Micro 5v;
2. Bluetooth module (Bluefruit & Arduino, iPad);
3. Connect the button;
4. Connect the two resistors;
5. Test MIDI library;
6. Test all together with independent power;
7. Record a video of the prototype;

## Material
We are going to need:
- A 5v Arduino;
- A [Bluefruit board](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration) (Bluefruit LE UART Friend);
- A bunch of jumpers;


# 1 - Get started on Arduino!
Because there was no cable (get it ASAP!), decided to use my old Arduino Uno (still working!) instead of the brand new Arduino Micro. I also updated Arduino's IDE software.

# 2 - Bluetooth module
First, I soldered the connectors into the board. Then, I tested to see if the module was working all right by following the tutorial [here](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/hidkeyboard).

In short, you'll need:

## Wiring
Just like in this image:
![wiring](https://cdn-learn.adafruit.com/assets/assets/000/025/182/medium800/adafruit_products_UARTFriend_bb.png)

## Setting Adafruit's BLE on Arduino
For this I used [this tutorial](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/software).

The tutorial is effective, covering things such as [configuring the board](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/configuration); sketch examples (e.g. using [AT commands](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-uart-friend/atcommand) from your computer to the board). By the end, you'll hopefully get your arduino, your BLE board, and you computer via serial port!

PS: Detailed documentation of AT commands can be found [here](https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/standard-at).

## Testing the BLE with a tablet/smartphone
You can test communication with your Android phone doing as demonstrate in [this video](https://www.youtube.com/watch?v=4hWlDKzn7tA).

## Testing MIDI messages over BLE to tablet/smartphone
The goal with this test is to continuously send the same MIDI note over BLE to a tablet, so that this note can be synthesized in the phone.

I STOPPED HERE!
